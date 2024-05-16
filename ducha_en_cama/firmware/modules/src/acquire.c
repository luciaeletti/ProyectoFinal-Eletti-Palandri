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
#define HEIGHT	900
#define FACTOR 3.76
#define CONVERSION 1000
#define GANANCIA 0.689
#define OFFSET -1.117

#define TEMP_BUS GPIO_NUM_1


DeviceAddress tempSensors[1]; 



/*==================[typedef]================================================*/

/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/

/*==================[internal functions definition]==========================*/
void ReadSensorData(){
 // CONDIC_FUNC_T *my_condition;
	
	//Medicion de nivel de liquido con FDC1004  
	//GetConditions(&my_condition);

	/*double valor_nivel_inicial;
	double valor_referencia_inicial;
	double valor_nivel;
	double valor_referencia;
	float level = 0;
	float level_litros = 0;

    FDC1004_Reset();
    FDC1004_Init(FDC1004_100HZ, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
	valor_referencia_inicial = FDC1004_getCapacitance(FDC1004_MEAS2);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
    valor_nivel_inicial = FDC1004_getCapacitance(FDC1004_MEAS1);

    FDC1004_configureMeasurementSingle(FDC1004_MEAS2, FDC1004_CIN2, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS2,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
	valor_referencia = FDC1004_getCapacitance(FDC1004_MEAS2);
    FDC1004_configureMeasurementSingle(FDC1004_MEAS1, FDC1004_CIN1, 0);
    FDC1004_triggerSingleMeasurement(FDC1004_MEAS1,FDC1004_100HZ);
    vTaskDelay(50/portTICK_PERIOD_MS);
    valor_nivel = FDC1004_getCapacitance(FDC1004_MEAS1);

    level = HEIGHT * (FACTOR * (valor_nivel - valor_nivel_inicial)/(valor_referencia - valor_referencia_inicial));
   	my_condition->level = level/CONVERSION;
    snprintf(my_condition->nivel, 10, "%.2f", my_condition->level); 

------------------------------------------------------------------------------*/
	//Medicion temperatura
	ds18b20_init(TEMP_BUS);
    ds18b20_setResolution(tempSensors,2,10);
	//my_condition->temperature = ds18b20_get_temp();
    //snprintf(my_condition->temperatura, 10, "%.2d", my_condition->temperature);

	//SetConditions(&my_condition);
 //   printf("El valor es %d.\n", my_condition->temperature);
   printf("El valor es %d.\n",15);
	
}

/*==================[external functions definition]==========================*/
/**
 * @brief 		Acquiring thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vAcquiringTask(void *pvParameters) {

	while(1){
       // ReadSensorData();
       float temp;
      	ds18b20_init(TEMP_BUS);
        ds18b20_setResolution(tempSensors,2,10);
        temp = ds18b20_get_temp();
        printf("El valor es %f.\n",temp);
		vTaskDelay(5000 /portTICK_PERIOD_MS);
	}


}
