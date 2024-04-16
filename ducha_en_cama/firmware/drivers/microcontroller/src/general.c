/** @file general.c
 *  @brief 
 *
 * @version		1.0
 * @date		15/04/2024
 * @author		Eletti Lucía Valentina
 * @note
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2024.
 */
/*==================[inclusions]=============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "general.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
uint16_t mySN=0; 
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void COMM_SetSN(uint16_t* sn)
{
	mySN=*sn;
}

void COMM_GetSN(uint16_t* sn)
{
	*sn=mySN;
}
/*==================[external functions definition]==========================*/

void COMM_CreateSemaphores(void) {
	vSemaphoreCreateBinary(xEnviromentFullSemaphore);
	vSemaphoreCreateBinary(xReceivedMsgSemaphore);
	vSemaphoreCreateBinary(xAnalysisDoneSemaphore);
	vSemaphoreCreateBinary(xKeyListenerSemaphore);
	vSemaphoreCreateBinary(xDataStoredSemaphore);
	vSemaphoreCreateBinary(xNewSessionSemaphore);

}
