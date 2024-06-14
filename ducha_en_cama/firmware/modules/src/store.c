/** @file Store.c
 *  @brief Store .
 *
 *  Acquirer is responsible for\n
 *  -Read data.\n
 *  -Send data.
 * @version		1.0
 * @date		03/06/2024
 * @author		Eletti Lucía Valentina
 * @note
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2024.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "gpio_mcu.h"
#include "ds3231.h"
#include "store.h"
#include "conditions.h"
#include "alarms.h"
#include "definitions.h"
/*==================[macros]=================================================*/
/*==================[typedef]================================================*/
/*==================[internal functions declaration]==========================*/
/*==================[external functions declaration]==========================*/
/*==================[internal functions definition]==========================*/
/*==================[external functions definition]==========================*/
