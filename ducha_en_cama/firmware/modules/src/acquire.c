/** @file Acquire.c
 *  @brief Acquires signals from sensors.
 *
 *  Acquirer is responsible for\n
 *  -Read data.\n
 *  -Send data.
 * @version		1.0
 * @date		15/04/2024
 * @author		Eletti Lucía Valentina
 * @note
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2024.
 */
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
#include "acquire.h"
#include "general.h"

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/

/*==================[internal functions definition]==========================*/
void ReadSensorData(void){
	
		

}

/*==================[external functions definition]==========================*/
/**
 * @brief 		Acquiring thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vAcquiringTask(void *pvParameters) {

	xSemaphoreTake(xNewSessionSemaphore,0);
	NewSession=false;

	while(1){

		if(!NewSession)	xSemaphoreTake(xNewSessionSemaphore,portMAX_DELAY);
        ReadSensorData();
		vTaskDelay(4000 /portTICK_PERIOD_MS);
	}

}
