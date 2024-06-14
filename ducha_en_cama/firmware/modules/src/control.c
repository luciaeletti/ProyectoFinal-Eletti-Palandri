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
#include "lwip/err.h"
#include "lwip/sys.h"
#include "gpio_mcu.h"
#include "ds3231.h"
#include "control.h"
#include "conditions.h"
#include "alarms.h"
#include "definitions.h"
/*==================[macros]=================================================*/
typedef enum
{
    LLENANDO,
    CALENTANDO,
    DUCHANDO,
	REPOSO
}ESTADOS_T;

typedef enum
{
    APAGADA,
	PRENDIDA
}ESTADOS_BOMBA_T;

/*==================[typedef]================================================*/
CONDIC_FUNC_T data;
ALARM_T my_alarm;
INFO_SHOWER_T my_info;
ESTADOS_T estado = REPOSO;
ESTADOS_BOMBA_T estado_bomba = APAGADA;
TIME_T tiempo_uso;

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

	while(1){
		switch (estado)
		{
		case LLENANDO:
		GetConditions(&data);
		GetInfoShower(&my_info);
		strcpy(my_info.state, "LLENANDO");
		SetInfoShower(&my_info);
		estado=CALENTANDO;
			break;
		case CALENTANDO:
		GetConditions(&data);
		GetInfoShower(&my_info);
		strcpy(my_info.state, "CALENTANDO");
		SetInfoShower(&my_info);
		if(data.temperature>TEMP_MAX){
			estado=DUCHANDO;
		}
			break;
		case DUCHANDO:
		GetConditions(&data);
		GetInfoShower(&my_info);
		strcpy(my_info.state, "DUCHANDO");
		SetInfoShower(&my_info);
			break;
		case REPOSO:
		GetInfoShower(&my_info);
		if(my_info.condition==TRUE){
			estado = LLENANDO;
		}
			break;
		}
		vTaskDelay(2000 /portTICK_PERIOD_MS);
	}

}

void vControlBombaTask(void *pvParameters){
    bool estado_pin;
	bool estado_pump = false;
 	struct tm start_time,  end_time;
	while(1){
	estado_pin = GPIORead(BUTTON_PUMP_PIN);
	if(estado_pin == false){
		estado_pump = !estado_pump;
	}
	printf("El valor es %d.\n", estado_pump);
    vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void vControlTiempoTask(void *pvParameters) {
	while(1){
	GetTime(&tiempo_uso);
 	ds3231_get_time(&tiempo_uso.my_rtc, &tiempo_uso.current_time);
	ESP_LOGI("EVENT", "hora actual %02d:%02d:%02d", tiempo_uso.current_time.tm_hour, tiempo_uso.current_time.tm_min, tiempo_uso.current_time.tm_sec);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

