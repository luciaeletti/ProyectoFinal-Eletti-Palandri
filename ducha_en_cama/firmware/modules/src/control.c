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
}ESTADOS_BAÑO_T;

typedef enum
{
    ENJUAGUE,
	DESAGOTE,
	INACTIVO
}ESTADOS_AUTOLAVADO_T;

typedef enum
{
    APAGADA,
	PRENDIDA
}ESTADOS_COMPONENTES_T;

/*==================[typedef]================================================*/
CONDIC_FUNC_T data;
ALARM_T my_alarm;
INFO_SHOWER_T my_info;
INFO_AUTOLAVADO_T my_autolav;
ESTADOS_BAÑO_T estado_baño = REPOSO;
ESTADOS_AUTOLAVADO_T estado_autolav = INACTIVO;
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
	
	my_info.state_shower=false;

	while(1){
		switch (estado_baño)
		{
		case LLENANDO:
		GetConditions(&data);
		//if(data.level>LEVEL_MAX){
		vTaskDelay(2000 /portTICK_PERIOD_MS);
		GetInfoShower(&my_info);
		my_info.process=1;
		SetInfoShower(&my_info);
		estado_baño=CALENTANDO;
		//}
			break;
		case CALENTANDO:
		//activar resistencia 
		GetConditions(&data);
		//if(data.temperature>TEMP_MAX){
		vTaskDelay(2000 /portTICK_PERIOD_MS);
		GetInfoShower(&my_info);
		my_info.process=2;
		SetInfoShower(&my_info);
		estado_baño=DUCHANDO;
		//}
			break;
		case DUCHANDO:
		GetConditions(&data);
		GetInfoShower(&my_info);
		//my_info.process=3;
		vTaskDelay(2000 /portTICK_PERIOD_MS);
		if(my_info.state_shower==false){
		estado_baño = REPOSO;
		}
			break;
		case REPOSO:
		//if(data.level>LEVEL_MIN){
		vTaskDelay(3000 /portTICK_PERIOD_MS);
		GetInfoShower(&my_info);
		my_info.process=0;
		SetInfoShower(&my_info);
	//	}
		GetInfoShower(&my_info);
		if(my_info.condition==TRUE){
		estado_baño = LLENANDO;
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
		ManejoBombaDucha(estado_pump);
	}
	printf("El valor es %d.\n", estado_pump);
    vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void ManejoBombaDucha(bool state){
	switch (state)
	{
	case APAGADA:
	GPIOOff(BOMBA_DUCHA);
    printf("bomba apagada \n");
	GetInfoShower(&my_info);
	my_info.state_pump_shower=0;
	SetInfoShower(&my_info);
		break;
	case PRENDIDA:
	GPIOOn(BOMBA_DUCHA);
    printf("bomba prendida \n");
	GetInfoShower(&my_info);
	my_info.state_shower=true;
	my_info.state_pump_shower=1;
	SetInfoShower(&my_info);
		break;
	default:
		break;
	}
}

void vControlAutolavadoTask(void *pvParameters) {
	while(1){
		switch (estado_autolav)
		{

		case ENJUAGUE:

			break;
		case DESAGOTE:

			break;
		case INACTIVO:
			break;
		}
		vTaskDelay(1000 /portTICK_PERIOD_MS);
	}
}

void vControlAspiradoraTask(void *pvParameters){
    bool state_pin;
	bool estado_asp=false;
 	struct tm start_time,  end_time;
	while(1){
	state_pin = GPIORead(BUTTON_ASP_PIN);
	if(state_pin == false){
		estado_asp = !estado_asp;
		ManejoAspiradora(estado_asp);
	}
	printf("El valor es asp %d.\n", estado_asp);
    vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void ManejoAspiradora(bool state){
	switch (state)
	{
	case APAGADA:
	GPIOOff(ASPIRADORA);
	GetInfoAutolavado(&my_autolav);
	my_autolav.autolav=1;
	my_autolav.state_autolav = false;
	SetInfoAutolavado(&my_autolav);
    printf("ASPIRADORA apagada \n");
		break;
	case PRENDIDA:
	GPIOOn(ASPIRADORA);
	GetInfoAutolavado(&my_autolav);
	my_autolav.state_autolav = true;
	SetInfoAutolavado(&my_autolav);
    printf("ASPIRADORA prendida \n");
		break;
	default:
		break;
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

