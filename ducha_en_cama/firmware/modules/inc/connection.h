/*
 * Connection.h
 *
 *  Created on: 15/04/2024
 *      Author: Prototipado
 */
#ifndef CONNECTION_H
#define CONNECTION_H
/*==================[inclusions]=============================================*/
#include <string.h>
#include <stdio.h>
#include "conditions.h"
#include "general.h"
/*==================[macros]=================================================*/
/*==================[typedef]================================================*/
/*==================[internal data declaration]==============================*/
/*==================[internal functions declaration]=========================*/
void vConnectionWFTask(void *pvParameters);
void vConnectionMQTTTask(void *pvParameters);
void smartconfig_example_task(void * parm);
/*==================[external functions declaration]=========================*/
void vConnectionTask(void *pvParameters);




#endif   #ifndef CONNECTION_H