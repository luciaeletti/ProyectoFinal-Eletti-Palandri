/*
 * Acquire.h
 *
 *  Created on: 15/04/2024
 *      Author: Prototipado
 */
#ifndef ACQUIRE_H
#define ACQUIRE_H

/*==================[inclusions]=============================================*/
#include <string.h>
#include <stdio.h>
#include "conditions.h"
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
void vAcquiringTask(void *pvParameters) ;
void ReadSensorData();
void ClearInactiveSensors(void);
/*==================[external functions declaration]=========================*/












#endif   #ifndef ACQUIRE_H