/*
 * Conditions.h
 *
 *  Created on: 15/04/2024
 *      Author: Prototipado
 */
#ifndef CONDITIONS_H
#define CONDITIONS_H

/*==================[inclusions]=============================================*/
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "ds3231.h"

/*==================[macros]=================================================*/
typedef struct{
DS3231_Info my_rtc;
struct tm current_time;
}TIME_T;

/**
 * @brief conditions definitions
 */
typedef struct{
float temperature;
char temp[3];
char lev[3];
float level;
}CONDIC_FUNC_T;

typedef struct{
int shower;
uint8_t condition;
char state[20];
uint8_t state_pump_shower;
bool state_shower;
}INFO_SHOWER_T;

typedef enum{
	FALSE,
	TRUE
}STATE_T;

typedef struct{
int shower;
uint8_t condition;
char state[20];
uint8_t state_pump_shower;
bool state_shower;
}INFO_AUTOLAVADO_T;

typedef enum{
	CONNECTION_WIFI_SMARTCONFIG,
    CONNECTION_WIFI_STA,
    STORE,
    COMUNICATION_MQTT,
    DISCONNECTION,
}CONNECTION_T;


typedef struct{
bool data_ok;
bool info_ok;
bool read_ok;
uint8_t SSID[33];
uint8_t PASSWORD[65];
}DATA_CONNECTION_T;


void SetConditions(CONDIC_FUNC_T* pCondition);
void GetConditions(CONDIC_FUNC_T* pCondition);
void SetInfoShower(INFO_SHOWER_T* pInfo);
void GetInfoShower(INFO_SHOWER_T* pInfo);
void SetInfoAutolavado(INFO_AUTOLAVADO_T* pInfoAutolav);
void GetInfoAutolavado(INFO_AUTOLAVADO_T* pInfoAutolav);
void SetInfoConnection(DATA_CONNECTION_T* pConnection);
void GetInfoConnection(DATA_CONNECTION_T* pConnection);
void SetTime(TIME_T* pTime);
void GetTime(TIME_T* pTime);

#endif   #ifndef CONDITIONS_H