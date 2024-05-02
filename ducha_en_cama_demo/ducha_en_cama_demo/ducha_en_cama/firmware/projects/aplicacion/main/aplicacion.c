/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
/*==================[inclusions]=============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "driver/gpio.h"
#include "LCD_I2C.h"
#include "driver/i2c.h"
#include "KeyPad.h"
#include "FDC1004.h"
#include "ds18b20.h"
#include "gpio_mcu.h"
/*==================[macros and typedef]================================================*/
#define	FALSE		0
#define	TRUE		1

#define NUM_ROWS	1 /*Cantidad de filas del teclado*/
#define NUM_COLS	3 /*Cantidad de columnas del teclado*/
#define LCD_ADDR  0x3f  /*!< slave address for LCD */
#define I2C_MASTER_SCL_IO   GPIO_NUM_7 /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO   GPIO_NUM_6/*!< gpio number for I2C master data  */

#define TEMP_BUS GPIO_NUM_1

#define UP_BUTTON_PIN 5
#define DOWN_BUTTON_PIN 10
#define SELECT_BUTTON_PIN 4
#define PUMP_PIN 11
#define PUMP_RECIRCUL_PIN 21
#define ASP_PIN 3
#define LED_TEMP GPIO_23

#define HEIGHT	900
#define FACTOR 3.76
#define CONVERSION 1000

#define TEMP_MIN 35
#define TEMP_MAX 20

#define LEVEL_MIN 5
#define LEVEL_MAX 20

DeviceAddress tempSensors[1]; 
float temperature;

double valor_nivel_inicial;
double valor_referencia_inicial;
double valor_nivel;
double valor_referencia;
float level = 0;
float level_litros = 0;

typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_SELECT,

} button_event_t;

button_event_t event;
TimerHandle_t button_timer;
QueueHandle_t button_queue;
uint8_t last_button_states[3]={1,1,1};
uint8_t selected = 0;
DeviceAddress tempSensors[1]; 
const char *menus[] = {"1.DUCHA", "2.AUTOLAVADO", "3.CONFIGURACION"};

/*==================[internal functions declaration]==========================*/
void button_timer_callback(TimerHandle_t xTimer);
void delayMs(const TickType_t mSec);
void print_menu(const char *menus[], int num_menus, int selected);
void sub_menu(uint8_t select);
void sub_menu_ducha();
void sub_menu_configuracion();
void sub_menu_autolavado(); 
void menu_principal();
/*==================[external functions declaration]==========================*/

/*==================[internal functions definition]==========================*/
void delayMs(const TickType_t mSec)
{
	vTaskDelay(mSec / portTICK_PERIOD_MS);
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
void medirtemperatura(){
    ds18b20_init(TEMP_BUS);
    ds18b20_setResolution(tempSensors,2,10); 
    //GPIOInit(LED_TEMP,GPIO_MODE_OUTPUT);
    bool estado;
    estado = GPIORead(SELECT_BUTTON_PIN);
    while(estado){
    temperature = ds18b20_get_temp(); 
    LCDI2C_clear();
    char temperatura[10]; 
    snprintf(temperatura, 10, "%.2f", temperature); 
    LCDI2C_setCursor(0,1);
    LCDI2C_print("Temperatura del agua:");
    LCDI2C_setCursor(0,2);
    LCDI2C_print(temperatura);
    delayMs(5000);
   /* int temp = temperature;
    if(temp>TEMP_MAX){
        GPIOOn(LED_TEMP);
    }*/
    }
}

void medirnivel(){

    FDC1004_Reset();
    //FDC1004_Init(FDC1004_100HZ, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
	valor_referencia_inicial = FDC1004_getCapacitance(FDC1004_MEAS2);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
    valor_nivel_inicial = FDC1004_getCapacitance(FDC1004_MEAS1);

    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
	valor_referencia = FDC1004_getCapacitance(FDC1004_MEAS2);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
    valor_nivel = FDC1004_getCapacitance(FDC1004_MEAS1);

    level = HEIGHT * (FACTOR * (valor_nivel - valor_nivel_inicial)/(valor_referencia - valor_referencia_inicial));

}

void manejoBombaDucha(uint8_t state){
    gpio_set_direction(PUMP_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(PUMP_PIN, state);
}

void  sub_menu_ducha(){

const char *sub_menu_ducha[] = {"1.DUCHA NORMAL", "2.DUCHA CON DESINF", "3.MENU ANTERIOR"};
int selected_shower = 0;
button_event_t event_shower;

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
                    LCDI2C_clear();
                    LCDI2C_setCursor(0,1);
                    LCDI2C_print("Llene el tanque");
                    LCDI2C_setCursor(0,2);
                    LCDI2C_print("de agua limpia");
                    delayMs(10000);
                    LCDI2C_clear();
                    medirnivel();
                    medirtemperatura();

				}

                    if (selected_shower == 1){
                       GPIOInit(LED_TEMP,GPIO_MODE_OUTPUT);
                       GPIOOn(LED_TEMP);
                  /*  gpio_set_direction(LED_TEMP, GPIO_MODE_OUTPUT);
  	                gpio_set_pull_mode(LED_TEMP, GPIO_PULLUP_ONLY);
                    gpio_set_level(LED_TEMP,true);*/
                      
				}
                        
					if (selected_shower == 2){
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
                    LCDI2C_clear();
                    LCDI2C_setCursor(0,1);
                    LCDI2C_print("Llene el tanque");
                    LCDI2C_setCursor(0,2);
                    LCDI2C_print("de agua limpia");
                    delayMs(15000);
                    LCDI2C_clear();
                    LCDI2C_setCursor(5,2);
                    LCDI2C_print("AUTOLAVADO");
					gpio_set_direction(PUMP_PIN, GPIO_MODE_OUTPUT);
                    gpio_set_level(PUMP_PIN, 1);
					}
                    if (selected_autolav== 1) {
                    LCDI2C_clear();
					gpio_set_direction(PUMP_RECIRCUL_PIN, GPIO_MODE_OUTPUT);
                    gpio_set_level(PUMP_RECIRCUL_PIN, 1);
                    LCDI2C_setCursor(0,1);
					LCDI2C_print("BOMBA RECIRCULACION");
                    LCDI2C_setCursor(6,2);
					LCDI2C_print("ACTIVADA");
					}
					if (selected_autolav == 2){
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
                    LCDI2C_clear();
					gpio_set_direction(ASP_PIN, GPIO_MODE_OUTPUT);
                    gpio_set_level(ASP_PIN, 1);
					LCDI2C_setCursor(0,1);
					LCDI2C_print("ASPIRADORA ACTIVADA");
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

void app_main(){
   
    
    LCDI2C_init(LCD_ADDR,20,4,I2C_MASTER_SDA_IO,I2C_MASTER_SCL_IO);
    
	LCDI2C_backlight();

    

 	gpio_set_direction(UP_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(DOWN_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(SELECT_BUTTON_PIN, GPIO_MODE_INPUT);

  	gpio_set_pull_mode(UP_BUTTON_PIN, GPIO_PULLUP_ONLY);
  	gpio_set_pull_mode(DOWN_BUTTON_PIN, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(SELECT_BUTTON_PIN, GPIO_PULLUP_ONLY);

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
	delayMs(5000);
	LCDI2C_clear();
	LCDI2C_setCursor(0,0);
	LCDI2C_print(" Elija una opcion: ");
 	print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
    printf("holaaaa");
    
   while (1) {
    printf("hola");
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



