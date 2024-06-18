/** @file Store.c
 *  @brief Store .
 *
 * @version		1.0
 * @date		14/06/2024
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
#include "store.h"
#include "definitions.h"
#include "at24c.h"


/*==================[macros]=================================================*/
/*==================[typedef]================================================*/
/*==================[internal functions declaration]==========================*/
/*==================[external functions declaration]==========================*/
/*==================[internal functions definition]==========================*/
/*==================[external functions definition]==========================*/
void ReadString(const char *data, uint16_t saddres, uint16_t eadrres){

    int lenghe = strlen(data);
 

}