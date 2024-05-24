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
#include "connection.h"
#include "gpio_mcu.h"
#include "i2c_mcu.h"
#include "acquire.h"
#include "analyzer.h"

/*==================[macros]=======================================*/
#define NUM_ROWS	1 /*Cantidad de filas del teclado*/
#define NUM_COLS	3 /*Cantidad de columnas del teclado*/
#define LCD_ADDR  0x3f  /*!< slave address for LCD */
#define I2C_MASTER_SCL_IO   GPIO_NUM_6 /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO   GPIO_NUM_7/*!< gpio number for I2C master data  */

#define LED_TEMP GPIO_2
#define HAB_I2C GPIO_19
//#define HEIGHT	900
//#define FACTOR 3.76
//#define CONVERSION 1000
/*
DeviceAddress tempSensors[1]; 
float temperature;
char temperatura[10]; 
int temp;
double valor_nivel_inicial;
double valor_referencia_inicial;
double valor_nivel;
double valor_referencia;
float level = 0;
float level_ok=0;
float level_litros = 0;
char nivel[10]; 

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

==================[internal functions declaration]==========================*/
/*void button_timer_callback(TimerHandle_t xTimer);
void delayMs(const TickType_t mSec);
void print_menu(const char *menus[], int num_menus, int selected);
void sub_menu(uint8_t select);
void sub_menu_ducha();
void sub_menu_configuracion();
void sub_menu_autolavado(); 
void menu_principal();
==================[external functions declaration]==========================*/

/*==================[internal functions definition]==========================*/

/*
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

    //GPIOInit(LED_TEMP,GPIO_MODE_OUTPUT);
   // bool estado;
    //estado = GPIORead(SELECT_BUTTON_PIN);
    //while(estado){
    temperature = ds18b20_get_temp(); 
    snprintf(temperatura, 10, "%.2f", temperature); 
    int temp = temperature;
    if(temp>TEMP_MAX){
        GPIOOn(LED_TEMP);
    }
    //}
}

void medirnivel(){
   
    ESP_LOGI(TAG, "entro a medir nivel");
    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
	valor_referencia_inicial = FDC1004_getCapacitance(FDC1004_MEAS2);
    printf("%f\n", valor_referencia_inicial);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
    valor_nivel_inicial = FDC1004_getCapacitance(FDC1004_MEAS1);
    printf("Valor de means: %d\n", FDC1004_MEAS1);
    printf("%f\n", valor_nivel_inicial);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
	valor_referencia = FDC1004_getCapacitance(FDC1004_MEAS2);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);

    vTaskDelay(50/portTICK_PERIOD_MS);
    valor_nivel = FDC1004_getCapacitance(FDC1004_MEAS1);

    level = (FACTOR  (valor_nivel - valor_nivel_inicial)/(valor_referencia - valor_referencia_inicial));
    printf("Valor de nivel: %f\n", level);
    level_ok = (level*GANANCIA)+OFFSET;
    printf("Valor de nivel ok: %f\n", level_ok);
    level_litros = (level_ok*900)/1000;
    printf("Valor de nivel ok litros: %f\n", level_litros);
    snprintf(nivel, 10, "%.2f", level); 
   // delayMs(5000);

}

void manejoBombaDucha(uint8_t state){
    gpio_set_direction(PUMP_PIN, GPIO_MODE_INPUT);
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
                    LCDI2C_setCursor(0,0);
                    LCDI2C_print("Para comenzar:");
                    LCDI2C_setCursor(1,1);
                    LCDI2C_print("LLENE EL TANQUE DE");
                    LCDI2C_setCursor(4,2);
                    LCDI2C_print("AGUA LIMPIA");
                    LCDI2C_setCursor(0,3);
                    LCDI2C_print("-------------------");
                    medirnivel();
                    delayMs(10000);
                    LCDI2C_clear();
                    //printf("Paso x medir nivel");
                    LCDI2C_setCursor(7,1);
                    LCDI2C_print("PUEDE");
                    LCDI2C_setCursor(6,2);
                    LCDI2C_print("COMENZAR");
                    delayMs(5000);
                    while(1){
                    LCDI2C_clear();
                    LCDI2C_setCursor(0,0);
                    LCDI2C_print("DUCHANDO...");
                    LCDI2C_setCursor(0,1);
                    LCDI2C_print("Temp del agua:");
                    LCDI2C_setCursor(15,1);
                    medirtemperatura();
                    LCDI2C_print(temperatura);
                    temp = temperature;
                    if(temp>TEMP_MAX){ 
                        GPIOOn(LED_TEMP);
                    
                    }
                    medirnivel();
                    LCDI2C_setCursor(0,2);
                    LCDI2C_print("Nivel del agua:");
                    LCDI2C_setCursor(16,2);
                    LCDI2C_print(nivel);
                    LCDI2C_setCursor(0,3);
                    LCDI2C_print("FINALIZA CON ESC");
                    delayMs(8000);
                    GPIOOff(LED_TEMP);
                    
				}
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
                    LCDI2C_setCursor(0,1);
                    LCDI2C_print("Llene el tanque");
                    LCDI2C_setCursor(0,2);
                    LCDI2C_print("de agua limpia");
                    delayMs(8000);
                    LCDI2C_clear();
                    LCDI2C_setCursor(5,2);
                    LCDI2C_print("AUTOLAVADO");
					gpio_set_direction(PUMP_PIN, GPIO_MODE_OUTPUT);
                    gpio_set_level(PUMP_PIN, 1);
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
                    LCDI2C_clear();
					LCDI2C_setCursor(0,1);
					LCDI2C_print("CONEXION EXITOSA");
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

*/
extern TaskHandle_t receiverHandler;
extern TaskHandle_t senderHandler;

extern TaskHandle_t receiverHandler2;
extern TaskHandle_t senderHandler2;


void app_main(){

    I2C_initialize(I2C_MASTER_FREQ_HZ);

    xTaskCreate(&vAcquiringTask, "adquirir", 65536, NULL, 1, &senderHandler);

    xTaskCreate(&vMonitoringTask, "monitoreo", 65536, NULL, 1, &receiverHandler);

    xTaskCreate(&vConnectionWFTask, "Connection WIFI", 32768, NULL, 1, &senderHandler2);
    
    xTaskCreate(&vConnectionMQTTTask, "Connection WIFI", 32768, NULL, 1, &receiverHandler2);

    printf("inicio menu \n");

    menuInit();

}














/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/*==================[internal data declaration]==============================*/
//TaskHandle_t senderHandler = NULL;
//TaskHandle_t receiverHandler = NULL;

/*==================[internal functions declaration]=========================*/

/*==================[external functions declaration]=========================*/

/*

void vConnectionWFTask(void *pvParameters){
    printf("entro a tarea");
    ESP_ERROR_CHECK(nvs_flash_init() );
    initialise_wifi();
    vTaskDelay(15000 /portTICK_PERIOD_MS);
    xTaskNotifyGive(receiverHandler);   

}*/


/*

void vConnectionMQTTTask(void *pvParameters)
{
    printf("entro a tarea 2");
while(1){
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
printf("se quedo aca 1");
    esp_log_level_set("*", ESP_LOG_INFO);
    printf("se quedo aca 2");
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_WS", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
   ESP_ERROR_CHECK(esp_netif_init());
   ESP_ERROR_CHECK(esp_event_loop_create_default());

   
    
       mqtt_app_start();
      vTaskDelay(10 /portTICK_PERIOD_MS);
    }

   
}

*/

/*
void app_main(void){
    
 // connection_wifi();
// vTaskDelay(20000 /portTICK_PERIOD_MS);
//    connection_mqtt();
    printf("holaaaaaa");
    xTaskCreate(&vConnectionWFTask, "Connection WIFI", 32768, NULL, 1, &senderHandler);
    printf("se creo tarea 1");
   
   xTaskCreate(&vConnectionMQTTTask, "Connection MQTT", 32768, NULL, 2, &receiverHandler);
    printf("se creo tarea 2");
    while(1){
    ESP_LOGI(TAG, "A ver si esto anda");
    printf("holaaaaaa");
    vTaskDelay(4000 /portTICK_PERIOD_MS);
    
}

*/

