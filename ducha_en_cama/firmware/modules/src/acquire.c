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
#include <math.h>
/*==================[macros]=================================================*/
#define HEIGHT	900
#define FACTOR 3.76
#define CONVERSION 1000
#define GANANCIA 0.48
#define OFFSET 0.8

//9.2 rayitas 10.36lt
//8 rayitas   8.88
//5 rayitas   5.81
//2 rayitas   2.65
//vacío       0.72 

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

/*==================[typedef]================================================*/

/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/

/*==================[internal functions definition]==========================*/
void ReadSensorData(){
	
	//Medicion de nivel de liquido con FDC1004  
	GetConditions(&my_condition);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 1);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
	valor_referencia = FDC1004_getCapacitance(FDC1004_MEAS2);
    valor_referencia= floorf(valor_referencia * 1000) / 1000;
    printf("valor_referencia: %.2f\n", valor_referencia);
    
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 2);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
  //  FDC_SetOffsetCalibration(FDC1004_CIN1, 5);
    vTaskDelay(50/portTICK_PERIOD_MS);
    valor_nivel = FDC1004_getCapacitance(FDC1004_MEAS1);
    valor_nivel= floorf(valor_nivel * 1000) / 1000;
    printf("valor_nivel: %.5f\n", valor_nivel);

    if(valor_referencia<1.8){
        level=0;
    } else {    level = ((valor_nivel - valor_nivel_inicial)/(valor_referencia - valor_referencia_inicial));}

   /* if(level<0){//puede ser negativo si hay variacion en valor de referencia inicial, puede existir pero debería ser muy pequeño
        level = 0;
    }*/
    //printf("Div: %.5f -- valor_nivel_inicial: %.5f --valor_referencia_inicial: %.5f, \n", level,valor_nivel_inicial, valor_referencia_inicial);
    level = (FACTOR * level);
    level_corregido = (level*GANANCIA)+OFFSET;
    level_super_corregido = ((level_corregido*900)/1000);
    my_condition.level = level_super_corregido;
	SetConditions(&my_condition);
	GetConditions(&my_condition);
    //printf("Nivel super: %.2f,level: %.2f, level_corregido: %.2f, \n", my_condition.level, level, level_corregido);
    printf("Nivel: %.2f lt \n", my_condition.level);
  //  snprintf(my_condition.lev, 10, "%.2f", my_condition.level); 

	//Medicion temperatura
	ds18b20_init(TEMP_BUS);
    ds18b20_setResolution(tempSensors,2,10);
	my_condition.temperature = ds18b20_get_temp();
    //snprintf(my_condition.temp, 10, "%.1f", my_condition.temperature);

	SetConditions(&my_condition);
    printf("Temp: %.2f ºC\n", my_condition.temperature);
	
}

/*==================[external functions definition]==========================*/
/**
 * @brief 		Acquiring thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vAcquiringTask(void *pvParameters) {

    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 1);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
	valor_referencia_inicial = FDC1004_getCapacitance(FDC1004_MEAS2);
    valor_referencia_inicial= floorf(valor_referencia_inicial * 1000) / 1000;
    printf("valor_referencia_inicial: %.2f\n", valor_referencia_inicial);
    //valor con el tanque vacio
    valor_referencia_inicial = 1.06;
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 2);//2*3.125=6.250
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
  //  FDC_SetOffsetCalibration(FDC1004_CIN1, 5);
    vTaskDelay(50/portTICK_PERIOD_MS);
    valor_nivel_inicial = FDC1004_getCapacitance(FDC1004_MEAS1);
    valor_nivel_inicial= floorf(valor_nivel_inicial * 1000) / 1000;
    printf("valor_nivel_inicial: %.2f\n", valor_nivel_inicial);
    valor_nivel_inicial = 2.74;


	while(1){
        ReadSensorData();
        //xTaskNotifyGive(receiverHandler);
		vTaskDelay(1000 /portTICK_PERIOD_MS);
	}


}
