/** @file Alarms.c
 * @version		1.0
 * @date		15/04/2024
 * @author		
 * @note
 * 
 *  Facultad de Ingenieria - UNER
 *  Laboratorio de prototipado Electronico &3D
 */
/*==================[inclusions]=============================================*/
#include "alarms.h"

/*==================[macros]=================================================*/
ALARM_T Alarm;
/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions declaration]==========================*/

void SetAlarms(ALARM_T* pAlarm){
	Alarm=*pAlarm;
}

void GetAlarms(ALARM_T* pAlarm){
	*pAlarm=Alarm;
}


