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
uint16_t temp_seteada = 37;
CONDIC_FUNC_T data;
ALARM_T my_alarm;
INFO_SHOWER_T my_info;
INFO_AUTOLAVADO_T my_autolav;
ESTADOS_BAÑO_T estado_baño = REPOSO;
ESTADOS_AUTOLAVADO_T estado_autolav = INACTIVO;
TIME_T tiempo_uso;
bool estado_pin;
bool estado_subir_temp;

bool estado_enter;
bool estado_pump = false;
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
my_info.ducha_init = false;

	while(1){
    	GetInfoShower(&my_info);
		switch (estado_baño)
		{
		case LLENANDO:
    //	printf("ESTADO LLENANDO \n");
			GetConditions(&data);
			if(data.level>2){
				GetInfoShower(&my_info);
				my_info.llenar_tanque=true;
				SetInfoShower(&my_info);
			}
			if(data.level>LEVEL_MAX){
				GetInfoShower(&my_info);
				my_info.tanque_lleno=true;
				SetInfoShower(&my_info);
			}/*
			estado_enter = GPIORead(SELECT_BUTTON_PIN);
			if(estado_enter == false){
				GetInfoShower(&my_info);
				my_info.process=1;
				SetInfoShower(&my_info);
				estado_baño=CALENTANDO;
			}*/
			while(!GPIORead(SELECT_BUTTON_PIN)){
				vTaskDelay(50 / portTICK_PERIOD_MS);
				estado_enter = true;
			}
			if(estado_enter){
				GetInfoShower(&my_info);
				my_info.process=1;
				SetInfoShower(&my_info);
				estado_enter = false;
				estado_baño=CALENTANDO;
			}
			break;
		case CALENTANDO:
    //	printf("ESTADO CALENTANDO \n");
		//DAR LA ORDEN A LA TAREA QUE CONTROLA RESISTENCIA PARA QUE LA PRENDA
			GetInfoShower(&my_info);
			my_info.resist_init=true;
			SetInfoShower(&my_info);
		/*	estado_enter = GPIORead(SELECT_BUTTON_PIN);
			if(estado_enter == false){
				GetInfoShower(&my_info);
				my_info.process=2;
				SetInfoShower(&my_info);
				estado_baño=DUCHANDO;
			}*/
			while(!GPIORead(SELECT_BUTTON_PIN)){
				vTaskDelay(50 / portTICK_PERIOD_MS);
				estado_enter = true;
			}
			if(estado_enter){
				GetInfoShower(&my_info);
				my_info.process=2;
				SetInfoShower(&my_info);
				estado_enter = false;
				estado_baño=DUCHANDO;
			}
			break;
		case DUCHANDO:
    //	printf("ESTADO DUCHANDO \n");
			GetInfoShower(&my_info);
			//estado_enter = GPIORead(SELECT_BUTTON_PIN);
			if(my_info.state_pump_shower==1 && my_info.state_shower==true){
				GetInfoShower(&my_info);
				GPIOOn(RESISTENCIA);
				GPIOOn(BOMBA_RECIRCULACION);
				my_info.process = 0;
				my_info.ducha_init = false;
				my_info.state_shower= false;
				my_info.resist_init = false;
				SetInfoShower(&my_info);
				estado_baño = REPOSO;
			}
			break;
		case REPOSO:
    //	printf("ESTADO REPOSO \n");
			GetInfoShower(&my_info);
			if(my_info.ducha_init == true){
				estado_baño = LLENANDO;
				}
			break;
		}	
		vTaskDelay(300 /portTICK_PERIOD_MS);
	}

}

void vControlBombaTask(void *pvParameters){
	while(1){
	//estado_pin = GPIORead(BUTTON_PUMP_PIN);
	while(!GPIORead(BUTTON_PUMP_PIN)){
		vTaskDelay(50 / portTICK_PERIOD_MS);
		estado_pin = true;
	}
	if(estado_pin){
	GetConditions(&data);
//	if(data.nivel_discreto<1)
	if(data.level<LEVEL_MIN){
    	printf("No hay suficiente agua para duchar \n");
		GetInfoShower(&my_info);
		my_info.shower=1;
		SetInfoShower(&my_info);
		estado_pump = 0;
	}
	else {
		estado_pump = !estado_pump;
		}
		estado_pin = false;
	}
	ManejoBombaDucha(estado_pump);
//	printf("El valor es %d.\n", estado_pump);
    vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}

void ManejoBombaDucha(bool state){
	switch (state)
	{
	case APAGADA:
		GPIOOn(BOMBA_DUCHA);
 	 //  printf("bomba apagada \n");
		GetInfoShower(&my_info);
		my_info.state_pump_shower=1;
		SetInfoShower(&my_info);
		break;
	case PRENDIDA:
		GetConditions(&data);
	if(data.level<LEVEL_MIN){
		estado_pump = 0;
	}
	else{
		GPIOOff(BOMBA_DUCHA);
  	//  printf("bomba prendida \n");
		GetInfoShower(&my_info);
		my_info.state_shower=true;
		my_info.state_pump_shower=0;
		SetInfoShower(&my_info);
		break;
		}
	}
}

void vControlTemperaturaTask(void *pvParameters){
	while(1){
		GetInfoShower(&my_info);
		if(my_info.resist_init == true){
	//		printf("ENTRO EN EL IF RESISTENCIA TRUE \n");
			GetConditions(&data);
			if(data.level<LEVEL_MIN){
				//se apaga por alto
				printf("NO CALENTANDO \n");
				GPIOOn(RESISTENCIA);
				GPIOOn(BOMBA_RECIRCULACION);
			}
			else{
				GPIOOff(BOMBA_RECIRCULACION);
				printf("CALENTANDO \n");
			//	GPIOOff(RESISTENCIA);
				ManejoTemperatura();
				data.temp_set=temp_seteada;
				SetConditions(&data);
				GetConditions(&data);
				printf("temperatura seteada: %.2d\n", temp_seteada);
				if(data.temperature<temp_seteada-2){
				//prenda la resistencia pq esta fuera del rango
					GPIOOff(RESISTENCIA);
				}
				if(data.temperature>temp_seteada+2){
					//apague la resistencia pq esta en el rango de temperatura aceptable
					GPIOOn(RESISTENCIA);
				}
				}
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

}

void ManejoTemperatura(){
	bool subir;
	bool bajar;
	bool tecla_up=false;
	bool tecla_down=false;
	//subir = GPIORead(UP_BUTTON_PIN);
	while(!GPIORead(UP_BUTTON_PIN)){
		vTaskDelay(50 / portTICK_PERIOD_MS);
		tecla_up = true;
	}
	if(tecla_up==true){
	//	printf("SUBIR TEMPERATURA \n");
		temp_seteada = temp_seteada +1;
        tecla_up =false;
	}

	while(!GPIORead(DOWN_BUTTON_PIN)){
		vTaskDelay(50 / portTICK_PERIOD_MS);
		tecla_down= true;
	}
	if(tecla_down==true){
	//	printf("BAJAR TEMPERATURA \n");
		temp_seteada = temp_seteada -1;
		tecla_down = false;
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
    bool state_pin=false;
	bool estado_asp=false;
	while(1){
	while(!GPIORead(BUTTON_ASP_PIN)){
		vTaskDelay(50 / portTICK_PERIOD_MS);
		state_pin = true;
	}
	if(state_pin){
		estado_asp = !estado_asp;
		state_pin = false;
	}
	ManejoAspiradora(estado_asp);
//	printf("El valor es asp %d.\n", estado_asp);
    vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}

void ManejoAspiradora(bool state){
	switch (state)
	{
	case APAGADA:
	//GPIOOff(ASPIRADORA);
	GPIOOn(ASPIRADORA);
	GetInfoAutolavado(&my_autolav);
	my_autolav.autolav=1;
	my_autolav.state_autolav = false;
	SetInfoAutolavado(&my_autolav);
 //   printf("ASPIRADORA apagada \n");
		break;
	case PRENDIDA:
	GPIOOff(ASPIRADORA);
	//GPIOOn(ASPIRADORA);
	GetInfoAutolavado(&my_autolav);
	my_autolav.state_autolav = true;
	SetInfoAutolavado(&my_autolav);
//    printf("ASPIRADORA prendida \n");
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

