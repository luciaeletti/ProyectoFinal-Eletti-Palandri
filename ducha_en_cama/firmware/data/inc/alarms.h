/*
 * Alarms.h
 *
 *  Created on: 15/04/2024
 *      Author: Prototipado
 */
#ifndef ALARMS_H
#define ALARMS_H

/*==================[inclusions]=============================================*/
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#define TEMP_MIN 35
#define TEMP_MAX 40

#define LEVEL_MIN 5
#define LEVEL_MAX 20

/*==================[macros]=================================================*/

typedef enum{
TEMPERATURE,
LEVEL
}ALARM_SOURCE_T;

typedef enum{
	OFF,
	ON
}ALARM_STATE_T;

typedef struct{
	ALARM_SOURCE_T source;
	ALARM_STATE_T state;
}ALARM_T;


void SetAlarms(ALARM_T* pCondition);
void GetAlarms(ALARM_T* pCondition);
void ResetAlarm(void);



#endif   #ifndef ALARMS_H