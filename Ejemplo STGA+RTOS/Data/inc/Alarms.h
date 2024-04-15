

#ifndef STGA_RTOS_DATA_INC_ALARMS_H_
#define STGA_RTOS_DATA_INC_ALARMS_H_

#include "lpc_types.h"

#define MAX_NUM_ALARMS 4

/**
 * @brief Flowmeter definitions
 */
typedef enum{
NONE,
TEMPERATURE,
HUMIDITY,
WINDSPEED,
FLOW,
POSITION
}ALARM_SOURCE_T;

/**
 * @brief Flowmeter definitions
 */
typedef enum{
	LOW,
	MEDIUM,
	HIGH
}ALARM_SCALE_T;

/**
 * @brief Flowmeter definitions
 */
typedef struct{
	ALARM_SOURCE_T source;
	ALARM_SCALE_T scale;
}ALARM_T;

Status ALARM_SetAlarm(ALARM_T* pAlarm);
Status ALARM_GetAlarm(ALARM_T vAlarm []);
Status ALARM_ResetAlarm(void);


#endif /* STGA_RTOS_DATA_INC_ALARMS_H_ */
