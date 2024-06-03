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

/*==================[macros]=================================================*/
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
char filling[20];
char heating[20];
char showering[20];
}INFO_SHOWER_T;

void SetConditions(CONDIC_FUNC_T* pCondition);
void GetConditions(CONDIC_FUNC_T* pCondition);
void SetInfoShower(INFO_SHOWER_T* pInfo);
void GetInfoShower(INFO_SHOWER_T* pInfo);




#endif   #ifndef CONDITIONS_H