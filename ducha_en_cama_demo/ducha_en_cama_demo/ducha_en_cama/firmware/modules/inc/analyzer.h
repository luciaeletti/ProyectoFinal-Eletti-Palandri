/*
 * Analyzer.h
 *
 *  Created on: 15/04/2024
 *      Author: Prototipado
 */
#ifndef ANALYZER_H
#define ANALYZER_H

/*==================[inclusions]=============================================*/
#include <string.h>
#include <stdio.h>
#include "conditions.h"
/*==================[macros]=================================================*/
#define TEMP_MIN 35
#define TEMP_MAX 40

#define LEVEL_MIN 5
#define LEVEL_MAX 20


/*==================[typedef]================================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
void vMonitoringTask(void *pvParameters);
/*==================[external functions declaration]=========================*/












#endif   #ifndef ANALYZER_H