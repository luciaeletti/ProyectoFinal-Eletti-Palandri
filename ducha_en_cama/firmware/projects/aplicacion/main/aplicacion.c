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
TaskHandle_t connection_handle = NULL;
/*==================[typedef]================================================*/


/*==================[internal data declaration]==============================*/


/*==================[internal functions declaration]=========================*/

/*==================[external functions declaration]=========================*/




void app_main(void){
    
    connection_wifi();
    vTaskDelay(60000 /portTICK_PERIOD_MS);
    connection_mqtt();
  //  xTaskCreate(&vConnectionTask, "Connection", 512, NULL, 5, &connection_handle);
    printf("hola");
}



