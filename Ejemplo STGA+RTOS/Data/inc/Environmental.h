/*
 * EnvironmentalData.h
 *
 *  Created on: 4/2/2016
 *      Author: Prototipado
 */

#ifndef STGA_RTOS_DATA_INC_ENVIRONMENTAL_H_
#define STGA_RTOS_DATA_INC_ENVIRONMENTAL_H_

#include "lpc_types.h"


/**
 * @brief GPS definitions
 */
typedef struct{

char  lat_deegres;
char  lat_minutes;
float lat_seconds;
unsigned char  long_deegres;
char  long_minutes;
float long_seconds;

float GPSSpeed;
float GPSDirection;

}ENV_GPS_T;

/**
 * @brief Thermohygrometer definitions
 */
typedef struct{
float temperature;
float humidity;
}ENV_HYT_T;

/**
 * @brief Anemometer definitions
 */
typedef struct{
 float speed;
 float AbsSpeed;
 unsigned short direction;
 char AbsDirection[3];
 float WsNS;
 float WsWE;

}ENV_WIND_T;

/**
 * @brief Flowmeter definitions
 */
typedef struct{
 short value;
 char on_off;
}ENV_FLOW_T;

/**
 * @brief Flowmeter definitions
 */
typedef struct{
	ENV_GPS_T gps;
	ENV_HYT_T tyh;
	ENV_WIND_T wind;
	ENV_FLOW_T flow;
}ENV_ENVIRONMENTAL_T;

Status ENV_SetCurrentEnvironmental(ENV_ENVIRONMENTAL_T* pEnvironment);
Status ENV_GetCurrentEnvironmental(ENV_ENVIRONMENTAL_T* pEnvironment);
Status ENV_SetPreviousEnvironmental(ENV_ENVIRONMENTAL_T* pEnvironment);
Status ENV_GetPreviousEnvironmental(ENV_ENVIRONMENTAL_T* pEnvironment);
Status ENV_ClearEnvironmental(void);

#endif /* STGA_RTOS_DATA_INC_ENVIRONMENTAL_H_ */
