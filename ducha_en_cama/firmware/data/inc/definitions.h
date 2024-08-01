/*
 * Definitions.h
 *
 *  Created on: 15/04/2024
 *      Author: Prototipado
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/*==================[inclusions]=============================================*/
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "gpio_mcu.h"

/*==================[macros]=================================================*/
#define UP_BUTTON_PIN 5
#define DOWN_BUTTON_PIN 10
#define SELECT_BUTTON_PIN 4
#define BUTTON_PUMP_PIN 11
#define BUTTON_ASP_PIN 3
#define BOMBA_DUCHA GPIO_22
#define ASPIRADORA GPIO_23
#define TEMP_BUS GPIO_NUM_1
#define WIFI_OK GPIO_20

#define LCD_ADDR 0x3f

#define LEVEL_MIN 7
#define LEVEL_MAX 20

#define TEMP_MIN 15
#define TEMP_MAX 35


#endif   #ifndef DEFINITIONS_H