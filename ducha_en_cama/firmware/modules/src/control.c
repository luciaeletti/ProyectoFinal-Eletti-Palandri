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
DS3231_Info *my_rtc = {0};


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
	bool estado_pump;
    int aux;
    while(1){ 
	estado_pin = GPIORead(BUTTON_PUMP_PIN);
	if(estado_pin==false){
	estado_pump = GPIORead(BOMBA_DUCHA);
	if(estado_pump==false){
		GPIOOn(BOMBA_DUCHA);
		printf("BOMBA PRENDIDA\n");
	}
	if(estado_pump==true){
		GPIOOff(BOMBA_DUCHA);
		printf("BOMBA APAGADA\n");
	}
	printf("El valor es %d.\n", estado_pump);
	}
	printf("Control de bomba \n");
  	vTaskDelay(500 /portTICK_PERIOD_MS);
    }
}
void ContarTiempo() {

	DS3231_Info my_rtc;
    ds3231_init_info(&my_rtc);
    printf("entro contar tiempo \n");
	
 	struct tm start_time, current_time, end_time;
	current_time.tm_sec = 0;
	current_time.tm_min = 29;
	current_time.tm_hour = 19;
 	ds3231_set_time(&my_rtc, &current_time);
	while(1){
ESP_LOGI("EVENT", "hora actual %02d:%02d:%02d", current_time.tm_hour, current_time.tm_min, current_time.tm_sec);

/*
    printf("1 \n");

 	ds3231_get_time(&my_rtc, &start_time);

    printf("2 \n");

    ESP_LOGI("EVENT", "Evento iniciado a las %02d:%02d:%02d", start_time.tm_hour, start_time.tm_min, start_time.tm_sec);
    printf("3 \n");
*/
    vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
   // printf("4 \n");

   // ds3231_get_time(&my_rtc, &end_time);
    
   // ESP_LOGI("EVENT", "Evento finalizado a las %02d:%02d:%02d", end_time.tm_hour, end_time.tm_min, end_time.tm_sec);
}