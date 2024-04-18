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
#include "general.h"
#include "interface.h"

/*==================[macros and typedef]================================================*/
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
                     
				}

                    if (selected_shower == 1){
                      
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
					gpio_set_direction(PUMP_PIN, GPIO_MODE_OUTPUT);
                    gpio_set_level(PUMP_PIN, 1);
                    LCDI2C_setCursor(3,1);
					LCDI2C_print("BOMBA DE DUCHA");
                    LCDI2C_setCursor(6,2);
					LCDI2C_print("ACTIVADA");
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

void menu_principal(){
   
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
/**
 * @brief 		Scre thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vScreeningTask(void *pvParameters) {

	xSemaphoreTake(xNewSessionSemaphore,0);
	NewSession=false;

	while(1){
		if(!NewSession)	xSemaphoreTake(xNewSessionSemaphore,portMAX_DELAY);
        menu_principal();
	//	vTaskDelay(4000 /portTICK_PERIOD_MS);
	}
    
}

