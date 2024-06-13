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

/*==================[macros]=================================================*/
typedef enum
{
    LLENANDO,
    CALENTANDO,
    DUCHANDO,
	REPOSO
}ESTADOS_T;

/*==================[typedef]================================================*/
CONDIC_FUNC_T data;
ALARM_T my_alarm;
INFO_SHOWER_T my_info;
ESTADOS_T estado = REPOSO;

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

void BombaDucha(){

    printf("llegooo \n");
	if(GPIORead(BOMBA_DUCHA)==false){
	GPIOOn(BOMBA_DUCHA);
    printf("bomba prendida \n");
	}
	if(GPIORead(BOMBA_DUCHA)==true){
	GPIOOff(BOMBA_DUCHA);
    printf("bomba apagada \n");
	}

	
}

void vControlBombaTask(void *pvParameters){

	while(1){
	GPIOActivInt(BUTTON_PUMP_PIN, &BombaDucha, false, NULL);
	vTaskDelay(2000 /portTICK_PERIOD_MS);
	}
}
void ContarTiempo() {

	DS3231_Info my_rtc;
    ds3231_init_info(&my_rtc);
    printf("entro contar tiempo \n");
	
 	struct tm start_time, end_time;
    printf("1 \n");

 	ds3231_get_time(&my_rtc, &start_time);

    printf("2 \n");

    ESP_LOGI("EVENT", "Evento iniciado a las %02d:%02d:%02d", start_time.tm_hour, start_time.tm_min, start_time.tm_sec);
    printf("3 \n");

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    printf("4 \n");

    ds3231_get_time(&my_rtc, &end_time);
    
    ESP_LOGI("EVENT", "Evento finalizado a las %02d:%02d:%02d", end_time.tm_hour, end_time.tm_min, end_time.tm_sec);
}