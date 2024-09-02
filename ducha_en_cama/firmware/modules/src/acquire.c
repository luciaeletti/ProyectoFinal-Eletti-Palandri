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
#include "definitions.h"
#include "analog_io_mcu.h"
#include <math.h>
/*==================[macros]=================================================*/
#define HEIGHT	900
#define FACTOR 3.76
#define CONVERSION 1000
#define GANANCIA 0.48
#define OFFSET 0.8


DeviceAddress tempSensors[1]; 
float level_corregido;
float level_super_corregido;
CONDIC_FUNC_T my_condition;

TaskHandle_t receiverHandler = NULL;
TaskHandle_t senderHandler = NULL;

float valor_nivel_inicial;
float valor_referencia_inicial;
float valor_nivel;
float valor_referencia;
float level = 0;
float level_litros = 0;
float canal_1;
float canal_2;
float canal_3;
float canal_4;

uint16_t dato_entrada;
float altura;
uint16_t area_base = 900;
float volumen;

 analog_input_config_t my_analog = {
        .input = CH2,
        .mode = ADC_SINGLE,

    };
/*==================[typedef]================================================*/

/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/
/*void ActivarSensores(float value, uint8_t sensor, uint8_t enable){
 //   GetConditions(&my_condition);
 	//  printf("entro a la fncion activar sensores \n");

    if(value>enable){
        my_condition.nivel_discreto|=sensor;

    } else {
        my_condition.nivel_discreto&=~sensor;
    }
    printf("sensor: %d-----nivel: %x \n",sensor, my_condition.nivel_discreto);
	//SetConditions(&my_condition);

}*/

/*==================[internal functions definition]==========================*/
double suma = 0;
int indice = 0;
float promedio_movil(uint16_t valor_actual) {
static float promedio;
    suma += valor_actual;
    if (indice >= 100) {
         promedio = suma / 100;
         indice = 0;
         suma = 0;
         altura = (promedio/5.81)-(154/5.81);
    volumen = (area_base*altura)/1000;
    //printf("EL NIVEL ES: %.2f\n",volumen);
    }
    indice++;
    return volumen;
}

void ReadSensorData(){ 
/*
	GetConditions(&my_condition);
//canal 1 primeros electrodos
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
   canal_1 = FDC1004_getCapacitance(FDC1004_MEAS1);
  //  canal_1 = floorf(valor_nivel * 1000) / 1000;
    printf("Canal Nº 1 - primeros electrodos: %.4f\n",canal_1 );
    ActivarSensores(canal_1,SENSOR_1,ACTIVATE_SENSOR_1);
//canal 2 segundos electrodos
    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
	canal_2 = FDC1004_getCapacitance(FDC1004_MEAS2);
   // canal_2 = floorf(valor_referencia * 1000) / 1000;
    printf("Canal Nº 2 - segundos electrodos: %.4f\n", canal_2);
    ActivarSensores(canal_2,SENSOR_2,ACTIVATE_SENSOR_2);

//canal 3 terceros electrodos
    FDC1004_configureMeasurementSingle(FDC1004_MEAS3, FDC1004_CIN3, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS3,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
   canal_3 = FDC1004_getCapacitance(FDC1004_MEAS3);
    //canal_3 = floorf(valor_nivel * 1000) / 1000;
    printf("Canal Nº 3 - terceros electrodos: %.4f\n", canal_3 );
    ActivarSensores(canal_3,SENSOR_3,ACTIVATE_SENSOR_3);
//canal 4 cuartos electrodos
    FDC1004_configureMeasurementSingle(FDC1004_MEAS4, FDC1004_CIN4, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS4,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
    canal_4 = FDC1004_getCapacitance(FDC1004_MEAS4);
 //   canal_4 = floorf(valor_nivel * 1000) / 1000;
    printf("Canal Nº 4 - cuartos electrodos: %.4f\n", canal_4);
    ActivarSensores(canal_4,SENSOR_4,ACTIVATE_SENSOR_4);
	ds18b20_init(TEMP_BUS);
    ds18b20_setResolution(tempSensors,2,10);
	my_condition.temperature = ds18b20_get_temp();
	SetConditions(&my_condition);
    printf("Temp: %.2f ºC\n", my_condition.temperature);
	printf("NIVEL DISCRETO ACTUAL  %d.\n", my_condition.nivel_discreto);

	SetConditions(&my_condition);*/
	GetConditions(&my_condition);
    AnalogInputReadSingle(CH2,&dato_entrada);
  //  printf("dato_entrada: %d\n",  dato_entrada);
    //promedio_movil(dato_entrada);
    my_condition.level = promedio_movil(dato_entrada);
	my_condition.temperature = ds18b20_get_temp();
    printf("my_condition.level: %.2f\n",  my_condition.level);
  //  printf("my_condition.temperature: %.2f\n", my_condition.temperature);
	SetConditions(&my_condition);

}

/*==================[external functions definition]==========================*/
/**
 * @brief 		Acquiring thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vAcquiringTask(void *pvParameters) {

 /*   FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
	valor_referencia_inicial = FDC1004_getCapacitance(FDC1004_MEAS2);
    valor_referencia_inicial= floorf(valor_referencia_inicial * 1000) / 1000;
   // printf("valor_referencia_inicial: %.2f\n", valor_referencia_inicial);
    //valor con el tanque vacio
  //  valor_referencia_inicial = 1.06;
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 0);//2*3.125=6.250
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
  //  FDC_SetOffsetCalibration(FDC1004_CIN1, 5);
    vTaskDelay(50/portTICK_PERIOD_MS);
    valor_nivel_inicial = FDC1004_getCapacitance(FDC1004_MEAS1);
    valor_nivel_inicial= floorf(valor_nivel_inicial * 1000) / 1000;
  //  printf("valor_nivel_inicial: %.2f\n", valor_nivel_inicial);
 //   valor_nivel_inicial = 2.74;

*/
    AnalogInputInit(&my_analog);
    ds18b20_init(TEMP_BUS);
    ds18b20_setResolution(tempSensors,2,10);
	while(1){
        ReadSensorData();
        //xTaskNotifyGive(receiverHandler);
		vTaskDelay(10 /portTICK_PERIOD_MS);
	}


}
