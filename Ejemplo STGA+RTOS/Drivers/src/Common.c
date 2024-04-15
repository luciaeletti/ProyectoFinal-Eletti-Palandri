/*
 * common.c
 *
 *  Created on: 25/1/2016
 *      Author: Prototipado
 */

#include "Common.h"
#include "Sensor.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
uint16_t mySN=0; /*serial number used on files name*/


void COMM_SetSN(uint16_t* sn)
{
	mySN=*sn;
}

void COMM_GetSN(uint16_t* sn)
{
	*sn=mySN;
}

/* Create the queues used by the modules  */
Status  COMM_CreateSemaphores(void) {

	vSemaphoreCreateBinary(xEnviromentFullSemaphore);
	vSemaphoreCreateBinary(xReceivedMsgSemaphore);
	vSemaphoreCreateBinary(xAnalysisDoneSemaphore);
	vSemaphoreCreateBinary(xKeyListenerSemaphore);
	vSemaphoreCreateBinary(xDataStoredSemaphore);
	vSemaphoreCreateBinary(xNewSessionSemaphore);

	return SUCCESS;
}
