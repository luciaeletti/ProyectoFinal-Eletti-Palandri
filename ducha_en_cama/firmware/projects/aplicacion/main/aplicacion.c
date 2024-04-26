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

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/*==================[internal data declaration]==============================*/
TaskHandle_t senderHandler = NULL;
TaskHandle_t receiverHandler = NULL;

/*==================[internal functions declaration]=========================*/
void vConnectionWFTask(void *pvParameters);
void vConnectionMQTTTask(void *pvParameters);

/*==================[external functions declaration]=========================*/

void vConnectionWFTask(void *pvParameters){
   // ESP_ERROR_CHECK( nvs_flash_init() );
    printf("holaaa");
    initialise_wifi();
    printf("hola");
    xTaskNotifyGive(receiverHandler);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
}

void vConnectionMQTTTask(void *pvParameters){
  
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_WS", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
 //   ESP_ERROR_CHECK(esp_netif_init());
 //   ESP_ERROR_CHECK(esp_event_loop_create_default());
    mqtt_app_start();
}


void app_main(void){
   //connection_wifi();
 //  vTaskDelay(60000 /portTICK_PERIOD_MS);
 //  connection_mqtt();
    xTaskCreate(&vConnectionWFTask, "Connection WIFI", 2048, NULL, 2, &senderHandler);
    xTaskCreate(&vConnectionMQTTTask, "Connection MQTT", 2048, NULL, 2, &receiverHandler);
    
}



