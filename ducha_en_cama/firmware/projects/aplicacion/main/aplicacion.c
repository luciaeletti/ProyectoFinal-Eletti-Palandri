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
#include "interface.h"

/*==================[macros]=================================================*/

#define	FALSE		0
#define	TRUE		1

#define LCD_ADDR  0x3f  /*!< slave address for LCD */
#define I2C_MASTER_SCL_IO    19  /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO    18  /*!< gpio number for I2C master data  */

#define NUM_ROWS	1 /*Cantidad de filas del teclado*/
#define NUM_COLS	3 /*Cantidad de columnas del teclado*/

#define UP_BUTTON_PIN 26
#define DOWN_BUTTON_PIN 27
#define SELECT_BUTTON_PIN 25
#define PUMP_PIN 33
#define PUMP_RECIRCUL_PIN 21
#define ASP_PIN 32

#define TEMP_BUS 23

static const char *TAG = "primerversion";

/*==================[typedef]================================================*/
typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_SELECT,

} button_event_t;

/*==================[internal data declaration]==============================*/

button_event_t event;
TimerHandle_t button_timer;
QueueHandle_t button_queue;
uint8_t last_button_states[3]={1,1,1};
uint8_t selected = 0;
DeviceAddress tempSensors[1]; 
const char *menus[] = {"1.MEDIR TEMP Y NIVEL", "2.ACTIVAR BOMBA", "3.ACTIVAR ASPIRADORA"};

/*==================[internal functions declaration]=========================*/
void button_timer_callback(TimerHandle_t xTimer);
void delayMs(const TickType_t mSec);
void print_menu(const char *menus[], int num_menus, int selected);
void sub_menu(uint8_t select);
void sub_menu_ducha();
void sub_menu_ducha_y_desinfec();
void sub_menu_autolavado(); 
/*==================[external functions declaration]=========================*/


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
            sub_menu_ducha_y_desinfec();
            break;
        case 2:
            sub_menu_autolavado();
            break; 
    }

}

void medirtemp(){
	                LCDI2C_clear();
                    
                    ds18b20_init(TEMP_BUS);
                    ds18b20_setResolution(tempSensors,2,10);

            while(1){
                    LCDI2C_clear();
                    float temperature = ds18b20_get_temp();
                    printf("Temperature: %0.1fC\n", temperature);
                    char temperaturaStr[10]; 
                    snprintf(temperaturaStr, 10, "%.2f C", temperature); 
                    LCDI2C_setCursor(0,1);
                    LCDI2C_print("La temperatura es: ");
                    LCDI2C_setCursor(0,2);
                    LCDI2C_print(temperaturaStr);
                    vTaskDelay(1000/portTICK_PERIOD_MS);

					}
                    	
}

void medirnivel(){
   
    LCDI2C_clear();
	            
uint8_t capdac_1 = 0;
uint8_t capdac_2 = 0;
uint8_t capdac_3 = 0;
double valor_nivel_inicial;
double valor_referencia_inicial;
double valor_ambiente;
double valor_nivel;
double valor_referencia;
float level = 0;
float level_litros=0;
double altura = 1;

uint16_t entero;

float valor_nivel_inicial_truncado;
float valor_nivel_truncado;
float valor_referencia_inicial_truncado;
float valor_referencia_truncado;
float valor_truncado;

    FDC1004_Reset();
    FDC1004_Init(FDC1004_100HZ, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);

    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    delayMs(50);
	valor_referencia_inicial = FDC1004_getCapacitance(FDC1004_MEAS2);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
    delayMs(50);
    valor_nivel_inicial = FDC1004_getCapacitance(FDC1004_MEAS1);


   while(1){

    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    delayMs(50);
	valor_referencia = FDC1004_getCapacitance(FDC1004_MEAS2);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
    delayMs(50);
    valor_nivel = FDC1004_getCapacitance(FDC1004_MEAS1);

    level = 900 * (3.76 * (valor_nivel - valor_nivel_inicial)/(valor_referencia - valor_referencia_inicial));
    level_litros = level/1000;
    
    LCDI2C_clear();
    char nivel_agua[10]; 
    snprintf(nivel_agua, 10, "%.2f", level_litros); 
    LCDI2C_setCursor(0,1);
    LCDI2C_print("El nivel de agua es: ");
    LCDI2C_setCursor(0,2);
    LCDI2C_print(nivel_agua);
    delayMs(3000);

}

                    	
}

void  sub_menu_ducha(){

const char *sub_menu_ducha[] = {"1.TEMPERATURA", "2.NIVEL DE AGUA", "3.MENU ANTERIOR"};
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
                        medirtemp();
				}

                    if (selected_shower == 1){
                        medirnivel();
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

void sub_menu_ducha_y_desinfec(){
const char *sub_menu_ducha_desinf[] = {"1.BOMBA DE DUCHA", "2.BOMBA RECIRCUL", "3.MENU ANTERIOR"};
int selected_shower_desinf = 0;
button_event_t event_shower_desinf;

LCDI2C_setCursor(0,0);
LCDI2C_print("Elija una opcion:");
print_menu(sub_menu_ducha_desinf, sizeof(sub_menu_ducha_desinf) / sizeof(sub_menu_ducha_desinf[0]), selected_shower_desinf);

 while (1) {
        if (xQueueReceive(button_queue, &event_shower_desinf, portMAX_DELAY)) {
            switch (event_shower_desinf) {
                case BUTTON_UP:
                    selected_shower_desinf = (selected_shower_desinf - 1 + sizeof(sub_menu_ducha_desinf) / sizeof(sub_menu_ducha_desinf[0])) % (sizeof(sub_menu_ducha_desinf) / sizeof(sub_menu_ducha_desinf[0]));
					print_menu(sub_menu_ducha_desinf, sizeof(sub_menu_ducha_desinf) / sizeof(sub_menu_ducha_desinf[0]), selected_shower_desinf);
                    break;
                case BUTTON_DOWN:
                    selected_shower_desinf = (selected_shower_desinf + 1) % (sizeof(sub_menu_ducha_desinf) / sizeof(sub_menu_ducha_desinf[0]));
					print_menu(sub_menu_ducha_desinf, sizeof(sub_menu_ducha_desinf) / sizeof(sub_menu_ducha_desinf[0]), selected_shower_desinf);
                    break;
                case BUTTON_SELECT:
                    if (selected_shower_desinf== 0) {
                    LCDI2C_clear();
					gpio_set_direction(PUMP_PIN, GPIO_MODE_OUTPUT);
                    gpio_set_level(PUMP_PIN, 1);
                    LCDI2C_setCursor(3,1);
					LCDI2C_print("BOMBA DE DUCHA");
                    LCDI2C_setCursor(6,2);
					LCDI2C_print("ACTIVADA");
					}
                    if (selected_shower_desinf== 1) {
                    LCDI2C_clear();
					gpio_set_direction(PUMP_RECIRCUL_PIN, GPIO_MODE_OUTPUT);
                    gpio_set_level(PUMP_RECIRCUL_PIN, 1);
                    LCDI2C_setCursor(0,1);
					LCDI2C_print("BOMBA RECIRCULACION");
                    LCDI2C_setCursor(6,2);
					LCDI2C_print("ACTIVADA");
					}
					if (selected_shower_desinf == 2){
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

const char *sub_menu_autolavado[] = {"1.ACTIVAR ASPIRADORA", "2.MENU ANTERIOR"};
int selected_autolavado = 0;
button_event_t event_autolavado;

LCDI2C_setCursor(0,0);
LCDI2C_print("Elija una opcion:");
print_menu(sub_menu_autolavado, sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0]), selected_autolavado);

 while (1) {
        if (xQueueReceive(button_queue, &event_autolavado, portMAX_DELAY)) {
            switch (event_autolavado) {
                case BUTTON_UP:
                    selected_autolavado= (selected_autolavado - 1 + sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0])) % (sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0]));
					print_menu(sub_menu_autolavado, sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0]), selected_autolavado);
                    break;
                case BUTTON_DOWN:
                    selected_autolavado = (selected_autolavado + 1) % (sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0]));
					print_menu(sub_menu_autolavado, sizeof(sub_menu_autolavado) / sizeof(sub_menu_autolavado[0]), selected_autolavado);
                    break;
                case BUTTON_SELECT:
                    if (selected_autolavado == 0) {
                    LCDI2C_clear();
					gpio_set_direction(ASP_PIN, GPIO_MODE_OUTPUT);
                    gpio_set_level(ASP_PIN, 1);
					LCDI2C_setCursor(0,1);
					LCDI2C_print("ASPIRADORA ACTIVADA");
					}
                    
					if (selected_autolavado == 1){
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


void app_main(void){
    
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
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



