#ifndef GENERAL_H
#define GENERAL_H
/*
 * General.h
 *
 *  Created on: 15/04/2024
 *      Author: Lucia Valentina Eletti 
 */
/*==================[inclusions]=============================================*/
#include <stdint.h>
#include <stdio.h>
#include "stdbool.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

/*==================[macros]=================================================*/
#define I2C_MASTER_SCL_IO    19  /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO    18  /*!< gpio number for I2C master data  */
#define LCD_ADDR  0x3f  /*!< slave address for LCD */
#define TEMP_BUS 23


/*==================[typedef]================================================*/

/*==================[internal data declaration]==============================*/
bool NewSession;
SemaphoreHandle_t xEnviromentFullSemaphore;
SemaphoreHandle_t xReceivedMsgSemaphore;
SemaphoreHandle_t xAnalysisDoneSemaphore;
SemaphoreHandle_t xKeyListenerSemaphore;
SemaphoreHandle_t xDataStoredSemaphore;
SemaphoreHandle_t xNewSessionSemaphore;
SemaphoreHandle_t xChangeStateSemaphore;
/*==================[internal functions declaration]=========================*/
void COMM_CreateSemaphores(void);
void COMM_SetSN(uint16_t* sn);
void COMM_GetSN(uint16_t* sn);



#endif /* GENERAL_H_ */
/*==================[end of file]============================================*/
