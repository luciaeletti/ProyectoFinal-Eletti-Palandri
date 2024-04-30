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
uint16_t temperature;
float level;
}CONDIC_FUNC_T;

void SetConditions(CONDIC_FUNC_T* pCondition);
void GetConditions(CONDIC_FUNC_T* pCondition);




#endif   #ifndef CONDITIONS_H