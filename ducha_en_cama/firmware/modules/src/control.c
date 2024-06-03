/** @file Control.c
 *  @brief Control .
 *
 *  Acquirer is responsible for\n
 *  -Read data.\n
 *  -Send data.
 * @version		1.0
 * @date		03/06/2024
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
#include "control.h"
#include "general.h"
#include "conditions.h"
#include "alarms.h"


/*==================[macros]=================================================*/
enum estados
{
    LLENANDO,
    CALENTANDO,
    DUCHANDO
}estado;

/*==================[typedef]================================================*/
CONDIC_FUNC_T data;
ALARM_T my_alarm;
INFO_SHOWER_T my_info;
/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/**
 * @brief 		Control thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vControlDuchaTask(void *pvParameters) {

	ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
	while(1){
		switch (estado)
		{
		case LLENANDO:
		GetConditions(&data);
		GetInfoShower(&my_info);
		strcpy(my_info.filling, "LLENANDO");
		SetInfoShower(&my_info);
		if(data.level>LEVEL_MAX){
			estado=CALENTANDO;
		}
			break;
		case CALENTANDO:
		GetConditions(&data);
		GetInfoShower(&my_info);
		strcpy(my_info.heating, "CALENTANDO");
		SetInfoShower(&my_info);
		if(data.temperature>TEMP_MAX){
			estado=DUCHANDO;
		}
			break;
		case DUCHANDO:
		GetConditions(&data);
		GetInfoShower(&my_info);
		strcpy(my_info.showering, "DUCHANDO");
		SetInfoShower(&my_info);

			break;
		}

		vTaskDelay(2000 /portTICK_PERIOD_MS);
	}

}
