/** @file Interface.c *
 * @version		1.0
 * @date		15/04/2024
 * @author		Eletti Lucía Valentina
 * @note
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2024.
 */
/*==================[inclusions]=============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "driver/gpio.h"
#include "LCD_I2C.h"
#include "driver/i2c.h"
#include "FDC1004.h"
#include "ds18b20.h"
#include "conditions.h"
#include "connection.h"
#include "interface.h"
#include "alarms.h"

//#define DEBUG_INTERFACE

/*==================[macros and typedef]================================================*/
typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_SELECT,
} button_event_t;

typedef enum
{
    LLENANDO,
    CALENTANDO,
    DUCHANDO,
}EVENTOS_DUCHA_T;

typedef enum
{
    PREPARADO,
    ASPIRACION,
    AUTOLAVADO,
    DESAGOTE,
    INACTIVO
}EVENTOS_AUTOLAVADO_T;

button_event_t event;
EVENTOS_DUCHA_T evento_ducha = LLENANDO;
EVENTOS_AUTOLAVADO_T evento_autolavado;
TimerHandle_t button_timer;
QueueHandle_t button_queue;

uint8_t last_button_states[3]={1,1,1};

uint8_t selected = 0;
uint8_t salir_ducha=0;
uint8_t mostrar=0;
uint8_t salir_autolavado=0;
int inicio_calentando=1;
int inicio_duchando=1;
int inicio_llenando=1;
int fin_llenado=1;
const char *menus[] = {"1.DUCHA", "2.AUTOLAVADO", "3.CONFIGURACION"};

uint8_t flag = 0; 
int selected_shower = 0;

CONDIC_FUNC_T my_cond_func;
INFO_SHOWER_T info;
INFO_AUTOLAVADO_T autolav;
DATA_CONNECTION_T connection;
/*==================[internal functions definition]==========================*/
void delayMs(const TickType_t mSec)
{
	vTaskDelay(mSec / portTICK_PERIOD_MS);
}

void eventos_ducha_normal(){
char cadena_temp[3];
char cadena_temp_set[3];
char cadena_level[4];

    switch (evento_ducha)
    {
                case LLENANDO:
                if(inicio_llenando==1){
                        LCDI2C_clear();
                        LCDI2C_setCursor(2,1);
                        LCDI2C_print("LLENE EL TANQUE"); 
                        LCDI2C_setCursor(2,2);
                        LCDI2C_print("CON AGUA LIMPIA");
                        inicio_llenando=0;
                    }
                        GetInfoShower(&info);
                        if(info.llenar_tanque==true && inicio_llenando==0){
                            LCDI2C_clear();
                            LCDI2C_setCursor(1,1);
                            LCDI2C_print("LLENANDO TANQUE...");
                            LCDI2C_setCursor(3,2);
                            LCDI2C_print("Nivel:");
                            info.llenar_tanque=false;
                            SetInfoShower(&info);
                            inicio_llenando=2;
                        }
                        if(inicio_llenando==2 && info.tanque_lleno==false){
                        GetConditions(&my_cond_func);
                        snprintf(cadena_level, 10, "%.1f", my_cond_func.level);
                        LCDI2C_setCursor(10,2);
                        LCDI2C_print(cadena_level);
                        LCDI2C_setCursor(15,2);
                        LCDI2C_print("lt");
                        }
                        if(info.tanque_lleno==true&&fin_llenado==1){
                            LCDI2C_clear();
                            LCDI2C_setCursor(7,0);
                            LCDI2C_print("TANQUE"); 
                            LCDI2C_setCursor(7,1);
                            LCDI2C_print("LLENO!");
                            LCDI2C_setCursor(3,2);
                            LCDI2C_print("Para continuar"); 
                            LCDI2C_setCursor(3,3);
                            LCDI2C_print("presione ENTER");
                            info.tanque_lleno=false;
                            fin_llenado=0;
                            SetInfoShower(&info);
                            inicio_llenando=3;          
                        }
                        if(info.process==1){ 
                        evento_ducha = CALENTANDO;
                        }  
                            break;
                case CALENTANDO:
                       if(inicio_calentando==1){
                            LCDI2C_clear();
                            LCDI2C_setCursor(1,0);
                            LCDI2C_print("CALENTANDO AGUA...");
                            LCDI2C_setCursor(1,1);
                            LCDI2C_print("TA:");
                            LCDI2C_setCursor(11,1);
                            LCDI2C_print("TS:");
                            LCDI2C_setCursor(3,2);
                            LCDI2C_print("Para continuar");
                            LCDI2C_setCursor(3,3);
                            LCDI2C_print("presione ENTER");
                            inicio_calentando=0;
                    }  
                        GetConditions(&my_cond_func);
                        snprintf(cadena_temp, 10, "%.1f", my_cond_func.temperature);
                        LCDI2C_setCursor(5,1);
                        LCDI2C_print(cadena_temp);
                        snprintf(cadena_temp_set, 10, "%.1f", my_cond_func.temp_set);
                        LCDI2C_setCursor(15,1);
                        LCDI2C_print(cadena_temp_set);
                        GetInfoShower(&info); 
                         if(info.process==2){
                            evento_ducha=DUCHANDO;
                        }   
                            break;
                case DUCHANDO:
                    GetInfoShower(&info); 
	               // printf("ESTADO PANTALLA EN DUCHANDO \n");
                    if(mostrar==0){
                        LCDI2C_clear(); 
                        LCDI2C_setCursor(3,2);
                        LCDI2C_print("PRESIONE DUCHA");
                        LCDI2C_setCursor(4,1);
                        LCDI2C_print("PARA COMENZAR");
                        mostrar=1;
                    }
                    if(info.state_pump_shower==0 && mostrar==1){
                        if(inicio_duchando==1){
                        LCDI2C_clear();
                        LCDI2C_setCursor(5,0);
                        LCDI2C_print("DUCHANDO...");
                        LCDI2C_setCursor(0,1);
                        LCDI2C_print("TA:");
                        LCDI2C_setCursor(8,1);
                        LCDI2C_print("C");
                        LCDI2C_setCursor(10,1);
                        LCDI2C_print("NA:");
                        LCDI2C_setCursor(18,1);
                        LCDI2C_print("lt");
                        LCDI2C_setCursor(3,2);
                        LCDI2C_print("Para finalizar"); 
                        LCDI2C_setCursor(3,3);
                        LCDI2C_print("presione DUCHA"); 
                        inicio_duchando=0;
                        }
                        GetConditions(&my_cond_func);
                        LCDI2C_setCursor(3,1);
                        snprintf(cadena_temp, 10, "%.1f", my_cond_func.temperature);
                        LCDI2C_print(cadena_temp);
                        LCDI2C_setCursor(13,1);
                        snprintf(cadena_level, 10, "%.1f", my_cond_func.level);
                        LCDI2C_print(cadena_level);
                        }
                        GetInfoShower(&info); 
                        if(info.process==0){
                            printf("llego a salir ducha \n");
                            info.ducha_init = false;
                            SetInfoShower(&info); 
                            salir_ducha=1;
                            mostrar=2;
                            LCDI2C_clear();
                            LCDI2C_setCursor(2,1);
                            LCDI2C_print("DUCHA FINALIZADA"); 
                            LCDI2C_setCursor(5,2);
                            LCDI2C_print("CON EXITO!");
                            vTaskDelay(500 /portTICK_PERIOD_MS);
                            selected_shower = 2;
                        }
                        break;
                    }
                }
            

void eventos_autolavado(){
    switch (evento_autolavado)
    {
            case PREPARADO:
                LCDI2C_clear();
                LCDI2C_setCursor(5,0);
                LCDI2C_print("PREPARE EL"); 
                LCDI2C_setCursor(6,1);
                LCDI2C_print("PRODUCTO");
                LCDI2C_setCursor(0,2);
                LCDI2C_print("Al finalizar oprima: ");
                LCDI2C_setCursor(5,3);
                LCDI2C_print("ASPIRACION");
	        GetInfoAutolavado(&autolav);
                if(autolav.state_autolav==true){
                evento_autolavado = ASPIRACION;
            }     
                break;
            case ASPIRACION:
                LCDI2C_clear();
                LCDI2C_setCursor(4,0);
                LCDI2C_print("ASPIRANDO..."); 
                LCDI2C_setCursor(3,1);
                LCDI2C_print("Para finalizar");
                LCDI2C_setCursor(5,2);
                LCDI2C_print("presione: ");
                LCDI2C_setCursor(5,3);
                LCDI2C_print("ASPIRACION");
                GetInfoAutolavado(&autolav);
                if(autolav.state_autolav==false && autolav.autolav==1){
                evento_autolavado = AUTOLAVADO;
                }     
                break;
            case AUTOLAVADO:
                LCDI2C_clear();
                LCDI2C_setCursor(5,1);
                LCDI2C_print("AUTOLAVADO"); 
                LCDI2C_setCursor(3,2);
                LCDI2C_print("EN PROCESO...");
                vTaskDelay(10000 /portTICK_PERIOD_MS);
                evento_autolavado = DESAGOTE;
                break;
            case DESAGOTE:
                LCDI2C_clear();
                LCDI2C_setCursor(3,1);
                LCDI2C_print("DESAGOTANDO..."); 
//aca desagota hasta que el nivel de agua llega al minimo establecido... y se apaga automaticamente la bomba de desagote
                vTaskDelay(10000 /portTICK_PERIOD_MS);
                LCDI2C_clear();
                LCDI2C_setCursor(5,1);
                LCDI2C_print("AUTOLAVADO"); 
                 LCDI2C_setCursor(6,2);
                LCDI2C_print("EXITOSO!"); 
                vTaskDelay(3000 /portTICK_PERIOD_MS);
                evento_autolavado = INACTIVO;
                break;
            case INACTIVO:
            LCDI2C_clear();
            salir_autolavado=1;
            menuPrincipal();
                break;
    }
}

void button_timer_callback(TimerHandle_t xTimer) {
    uint8_t button_states[] = {
        gpio_get_level(UP_BUTTON_PIN),
        gpio_get_level(DOWN_BUTTON_PIN),
        gpio_get_level(SELECT_BUTTON_PIN)
    };

    for (int i = 0; i < 3; i++) {
        if (last_button_states[i] != button_states[i] && button_states[i] == 0) {
			button_event_t event = (button_event_t)i;
            xQueueSend(button_queue, &event, portMAX_DELAY);
        }
        last_button_states[i] = button_states[i];
    }
}

void print_menu(const char *menus[], int num_menus, int selected) {
    for (int i=0; i < num_menus; i++) {
        LCDI2C_setCursor(0, i+1);
       if (i == selected) {
            LCDI2C_print(">");
        } else {
            LCDI2C_print(" ");
        }
    LCDI2C_print(menus[i]);
    }
}

void sub_menu(uint8_t select){ 
    LCDI2C_clear();
    switch (select) {
        case 0:
            sub_menu_ducha();
            break;
        case 1:
            sub_menu_autolavado();
            break;
        case 2:
            sub_menu_configuracion();
            break; 
    }
}

void sub_menu_ducha(){
const char *sub_menu_ducha[] = {"1.DUCHA NORMAL", "2.DUCHA CON DESINF", "3.MENU ANTERIOR"};
button_event_t event_shower;

LCDI2C_clear();
LCDI2C_setCursor(0,0);
LCDI2C_print("Elija una opcion:");
print_menu(sub_menu_ducha, sizeof(sub_menu_ducha) / sizeof(sub_menu_ducha[0]), selected_shower);

 while (1) {
        if (xQueueReceive(button_queue, &event_shower, portMAX_DELAY)) {
            switch (event_shower) {
                case BUTTON_UP:
                    selected_shower= (selected_shower - 1 + sizeof(sub_menu_ducha) / sizeof(sub_menu_ducha[0])) % (sizeof(sub_menu_ducha) / sizeof(sub_menu_ducha[0]));
					print_menu(sub_menu_ducha, sizeof(sub_menu_ducha) / sizeof(sub_menu_ducha[0]), selected_shower);
                    break;
                case BUTTON_DOWN:
                    selected_shower = (selected_shower + 1) % (sizeof(sub_menu_ducha) / sizeof(sub_menu_ducha[0]));
					print_menu(sub_menu_ducha, sizeof(sub_menu_ducha) / sizeof(sub_menu_ducha[0]), selected_shower);
                    break;
                case BUTTON_SELECT:
                    if (selected_shower == 0) {
                    GetInfoShower(&info);
                    info.ducha_init = true;
                    SetInfoShower(&info);
                    evento_ducha = LLENANDO;
                    salir_ducha = 0;
                    while(salir_ducha==0){
                        eventos_ducha_normal();
                        vTaskDelay(100 /portTICK_PERIOD_MS);
        			    }
                    GetInfoShower(&info);
                    info.ducha_init = false;
                    info.process=50;
                    SetInfoShower(&info); 
                    }

                    if (selected_shower == 1){
				    }
                        
					if (selected_shower == 2){
					LCDI2C_clear();
					LCDI2C_setCursor(0,0);
					LCDI2C_print(" Elija una opcion: ");
 					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    while (1)
                    {
                        if (xQueueReceive(button_queue, &event, portMAX_DELAY))
                        {
                            switch (event)
                            {
                            case BUTTON_UP:
                                selected = (selected - 1 + sizeof(menus) / sizeof(menus[0])) % (sizeof(menus) / sizeof(menus[0]));
                                print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                                break;
                            case BUTTON_DOWN:
                                selected = (selected + 1) % (sizeof(menus) / sizeof(menus[0]));
                                print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                                break;
                            case BUTTON_SELECT:
                                sub_menu(selected);
                                break;
                            }
                        }
                    }
                }
            break;
            }
            
        }
}

}

void sub_menu_autolavado(){
const char *sub_menu_autolavado[] = {"1.REALIZAR AUTOLAV.", "2.MENU ANTERIOR"};
int selected_autolav = 0;
button_event_t event_autolav;

LCDI2C_setCursor(0,0);
LCDI2C_print("Elija una opcion:");
print_menu(sub_menu_autolavado, sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0]), selected_autolav);

 while (1) {
        if (xQueueReceive(button_queue, &event_autolav, portMAX_DELAY)) {
            switch (event_autolav) {
                case BUTTON_UP:
                    selected_autolav = (selected_autolav - 1 + sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0])) % (sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0]));
					print_menu(sub_menu_autolavado, sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0]), selected_autolav);
                    break;
                case BUTTON_DOWN:
                    selected_autolav = (selected_autolav + 1) % (sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0]));
					print_menu(sub_menu_autolavado, sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0]), selected_autolav);
                    break;
                case BUTTON_SELECT:
                    if (selected_autolav== 0) {
                    evento_autolavado = PREPARADO;   
                while(salir_autolavado==0){
                    eventos_autolavado();
                    vTaskDelay(1000 /portTICK_PERIOD_MS);
                   }
					}
					if (selected_autolav == 1){
					LCDI2C_clear();
					LCDI2C_setCursor(0,0);
					LCDI2C_print(" Elija una opcion: ");
 					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
		 while (1) {
       		if (xQueueReceive(button_queue, &event, portMAX_DELAY)) {
            switch (event) {
                case BUTTON_UP:
                    selected = (selected - 1 + sizeof(menus) / sizeof(menus[0])) % (sizeof(menus) / sizeof(menus[0]));
					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    break;
                case BUTTON_DOWN:
                    selected = (selected + 1) % (sizeof(menus) / sizeof(menus[0]));
					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    break;
                case BUTTON_SELECT:
                    sub_menu(selected);
                    break;
            }
            
        }
    }
}
            break;
            }
            
        }
    }
}

void sub_menu_configuracion(){

const char *sub_menu_configuracion[] = {"1.CONEXION WIFI", "2.OTROS AJUSTES", "3.MENU ANTERIOR"};
int selected_config= 0;
button_event_t event_config;

LCDI2C_setCursor(0,0);
LCDI2C_print("Elija una opcion:");
print_menu(sub_menu_configuracion, sizeof(sub_menu_configuracion) / sizeof(sub_menu_configuracion[0]), selected_config);

 while (1) {
        if (xQueueReceive(button_queue, &event_config, portMAX_DELAY)) {
            switch (event_config) {
                case BUTTON_UP:
                    selected_config= (selected_config - 1 + sizeof(sub_menu_configuracion) / sizeof(sub_menu_configuracion[0])) % (sizeof(sub_menu_configuracion) / sizeof(sub_menu_configuracion[0]));
					print_menu(sub_menu_configuracion, sizeof(sub_menu_configuracion) / sizeof(sub_menu_configuracion[0]), selected_config);
                    break;
                case BUTTON_DOWN:
                    selected_config = (selected_config + 1) % (sizeof(sub_menu_configuracion) / sizeof(sub_menu_configuracion[0]));
					print_menu(sub_menu_configuracion, sizeof(sub_menu_configuracion) / sizeof(sub_menu_configuracion[0]), selected_config);
                    break;
                case BUTTON_SELECT:
                    if (selected_config == 0) {
                    uint8_t aux=0;
                    LCDI2C_clear();
                    xTaskCreate(&vConnectionApp, "Connection", 32768, NULL, 1, NULL);
                    while(aux==0){
                    LCDI2C_setCursor(5,1);
					LCDI2C_print("CONECTANDO");
                    LCDI2C_setCursor(5,2);
					LCDI2C_print("A WIFI... ");
                    GetInfoConnection(&connection);
                    if(connection.data_ok==true){
                        aux=1;
                    LCDI2C_clear();
                    LCDI2C_setCursor(6,1);
					LCDI2C_print("CONEXION");
                    LCDI2C_setCursor(6,2);
					LCDI2C_print("EXITOSA!");
                    vTaskDelay(1000 /portTICK_PERIOD_MS);
                    }
                    vTaskDelay(1000 /portTICK_PERIOD_MS);
                    }
                    LCDI2C_clear();
					LCDI2C_setCursor(0,0);
					LCDI2C_print(" Elija una opcion: ");
 					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
		 while (1) {
       		if (xQueueReceive(button_queue, &event, portMAX_DELAY)) {
            switch (event) {
                case BUTTON_UP:
                    selected = (selected - 1 + sizeof(menus) / sizeof(menus[0])) % (sizeof(menus) / sizeof(menus[0]));
					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    break;
                case BUTTON_DOWN:
                    selected = (selected + 1) % (sizeof(menus) / sizeof(menus[0]));
					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    break;
                case BUTTON_SELECT:
                    sub_menu(selected);
                    break;
            }
            
        }
    }
}                    
                
					if (selected_config == 1){
					LCDI2C_clear();
					LCDI2C_setCursor(0,0);
					LCDI2C_print(" Elija una opcion: ");
 					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
		 while (1) {
       		if (xQueueReceive(button_queue, &event, portMAX_DELAY)) {
            switch (event) {
                case BUTTON_UP:
                    selected = (selected - 1 + sizeof(menus) / sizeof(menus[0])) % (sizeof(menus) / sizeof(menus[0]));
					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    break;
                case BUTTON_DOWN:
                    selected = (selected + 1) % (sizeof(menus) / sizeof(menus[0]));
					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    break;
                case BUTTON_SELECT:
                    sub_menu(selected);
                    break;
            }
            
        }
    }
}
            break;
            }
            
        }
    }
}

void menuInit(){
   
    LCDI2C_init(LCD_ADDR,20,4);
	LCDI2C_backlight();
    gpio_set_direction(UP_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(DOWN_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(SELECT_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_ASP_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_PUMP_PIN, GPIO_MODE_INPUT);

  	gpio_set_pull_mode(UP_BUTTON_PIN, GPIO_PULLUP_ONLY);
  	gpio_set_pull_mode(DOWN_BUTTON_PIN, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(SELECT_BUTTON_PIN, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(BUTTON_ASP_PIN, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(BUTTON_PUMP_PIN, GPIO_PULLUP_ONLY);

 	button_queue = xQueueCreate(10, sizeof(button_event_t));
  	button_timer = xTimerCreate("button_timer", pdMS_TO_TICKS(100), pdTRUE, (void *)0, button_timer_callback);
    xTimerStart(button_timer, 0);

	LCDI2C_setCursor(0,0);
	LCDI2C_print("********************");
	LCDI2C_setCursor(0,1);
	LCDI2C_print("      SISTEMA ");
	LCDI2C_setCursor(0,2);
	LCDI2C_print("   DUCHA EN CAMA ");
	LCDI2C_setCursor(0,3);
	LCDI2C_print("********************");
	delayMs(4000);
    menuPrincipal();
}

 void menuPrincipal(){
	LCDI2C_clear();
	LCDI2C_setCursor(0,0);
	LCDI2C_print(" Elija una opcion: ");
 	print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
    while (1) {
        if (xQueueReceive(button_queue, &event, portMAX_DELAY)) {
            switch (event) {
                case BUTTON_UP:
                    selected = (selected - 1 + sizeof(menus) / sizeof(menus[0])) % (sizeof(menus) / sizeof(menus[0]));
					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    break;
                case BUTTON_DOWN:
                    selected = (selected + 1) % (sizeof(menus) / sizeof(menus[0]));
					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    break;
                case BUTTON_SELECT:
                    sub_menu(selected);
                    break;
            }
            
        }

}
}

/*==================[external functions definition]==========================*/
