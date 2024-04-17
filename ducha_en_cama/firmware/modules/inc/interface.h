/** @file Interface.h
 *  @brief It handle user's input-Output, including generic display and keypad.
 * @version		1.0
 * @date		15/04/2024
 * @author		Lucía Valentina Eletti
 * @note
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 */
#ifndef INTERFACE_H
#define INTERFACE_H
/*==================[inclusions]=============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "lwip/err.h"
#include "lwip/sys.h"
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/
void vScreeningTask(void *pvParameters);
void InterfaceInit(void);









#endif   #ifndef INTERFACE_H