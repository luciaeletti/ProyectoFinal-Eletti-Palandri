/*
 * control.h
 *
 *  Created on: 03/06/2024
 *      Author: Prototipado
 */
#ifndef CONTROL_H
#define CONTROL_H

/*==================[inclusions]=============================================*/
#include <string.h>
#include <stdio.h>
#include "conditions.h"
#include "ds3231.h"

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions declaration]=========================*/
void vControlDuchaTask(void *pvParameters);
void vControlTiempoTask(void *pvParameters);







#endif #ifndef CONTROL_H