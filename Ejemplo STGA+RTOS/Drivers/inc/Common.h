/*
 * Common.h
 *
 *  Created on: 25/1/2016
 *      Author: Prototipado
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#include "chip.h"
#include <stdio.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "lpc_types.h"

Bool NewSession;/*main switch on/off for acquiring task */

xSemaphoreHandle xEnviromentFullSemaphore;
xSemaphoreHandle xReceivedMsgSemaphore;
xSemaphoreHandle xAnalysisDoneSemaphore;
xSemaphoreHandle xKeyListenerSemaphore;
xSemaphoreHandle xDataStoredSemaphore;
xSemaphoreHandle xNewSessionSemaphore;
xSemaphoreHandle xChangeStateSemaphore;

Status COMM_CreateSemaphores(void);
void COMM_SetSN(uint16_t* sn);
void COMM_GetSN(uint16_t* sn);

#endif /* INC_COMMON_H_ */
