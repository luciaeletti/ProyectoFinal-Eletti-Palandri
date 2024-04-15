/*
 * Condiciones.h
 *
 *  Created on: 15/04/2024
 *      Author: Prototipado
 */
#ifndef CONDICIONES_H
#define CONDICIONES_H

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







#endif   #ifndef CONDICIONES_H