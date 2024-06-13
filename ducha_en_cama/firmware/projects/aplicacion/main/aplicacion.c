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
#include "ds3231.h"
#include "at24c.h"
#include "interface.h"
#include "connection.h"
#include "gpio_mcu.h"
#include "i2c_mcu.h"
#include "acquire.h"
#include "analyzer.h"
#include "control.h"
#include "definitions.h"
/*==================[macros]=======================================*/

/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/

/*==================[internal functions definition]==========================*/

extern TaskHandle_t receiverHandler;
extern TaskHandle_t senderHandler;

extern TaskHandle_t receiverHandler2;
extern TaskHandle_t senderHandler2;
EEPROM_t *my_memory;

void app_main(){


 
    I2C_initialize(I2C_MASTER_FREQ_HZ);
/*uint8_t a = 24;
uint8_t b = 48;
uint8_t c = a+b;
uint8_t uno=0;
uint8_t dos=0;
uint8_t tres=0;



  //  xTaskCreate(&vAcquiringTask, "adquirir", 65536, NULL, 1, &senderHandler);

    //xTaskCreate(&vMonitoringTask, "monitoreo", 65536, NULL, 1, &receiverHandler);

   // xTaskCreate(&vConnectionWIFI, "WIFI", 32768, NULL, 1, NULL);
   
   // xTaskCreate(&vConnectionApp, "Connection", 32768, NULL, 1, NULL);

 //   xTaskCreate(&vControlDuchaTask, "CONTROL", 32768, NULL, 1, NULL);

  //  ContarTiempo();
        InitRom(&my_memory);
        esp_err_t err2 = WriteRom(&my_memory,0,a);
           if (err2 == ESP_OK) {
        printf("OKK escritura %d.\n", a);
    } else {
        printf(" CAGADA%d.\n", err2);
    }
        WriteRom(&my_memory,1,b);
        WriteRom(&my_memory,2,c);
        esp_err_t err = ReadRom(&my_memory, 0, &uno);
          if (err == ESP_OK) {
        printf("OKK lectura  %d.\n", uno);
    } else {
        printf("FAILLLL %d.\n", err);
    }

        //uint8_t aux2 = 0;
        //aux2 = ReadRom(&my_memory, 1, &dos);
       // uint8_t aux3 = 0;
       // aux3 = ReadRom(&my_memory, 2, &tres);
      //  printf("El valor es %d.\n", aux1);
      //  printf("El valor es %d.\n", aux2);
      //  printf("El valor es %d.\n", aux3);
*/
    GPIOInit(BOMBA_DUCHA, GPIO_OUTPUT);
    GPIOInit(BUTTON_PUMP_PIN, GPIO_INPUT);
    xTaskCreate(&vControlBombaTask, "BOMBA DUCHA", 32768, NULL, 1, NULL);


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

