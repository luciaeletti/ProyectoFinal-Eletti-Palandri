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
#include "analyzer.h"
#include "conditions.h"
#include "general.h"
#include "alarms.h"

/*==================[macros]=================================================*/
CONDIC_FUNC_T my_Data;
ALARM_T my_Alarm;
/*==================[typedef]================================================*/

/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/

/*==================[internal functions definition]==========================*/
void EvaluateRanges(){

	GetConditions(&my_Data);

	if(my_Data.temperature>TEMP_MAX || my_Data.temperature<TEMP_MIN){
		my_Alarm.source=TEMPERATURE;
		my_Alarm.state=ON;
		SetAlarms(&my_Alarm);
	}

	if(my_Data.level>LEVEL_MAX || my_Data.level<LEVEL_MIN){
		my_Alarm.source=LEVEL;
		my_Alarm.state=ON;
		SetAlarms(&my_Alarm);
	}

}
/*==================[external functions definition]==========================*/
/**
 * @brief 		Monitoring thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vMonitoringTask(void *pvParameters){

	ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

	while(1){
        EvaluateRanges();
		vTaskDelay(4000 /portTICK_PERIOD_MS);
	}
	
}
