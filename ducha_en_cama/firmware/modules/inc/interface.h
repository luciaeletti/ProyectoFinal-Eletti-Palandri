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
#define	FALSE		0
#define	TRUE		1

#define NUM_ROWS	1 /*Cantidad de filas del teclado*/
#define NUM_COLS	3 /*Cantidad de columnas del teclado*/

#define UP_BUTTON_PIN 26
#define DOWN_BUTTON_PIN 27
#define SELECT_BUTTON_PIN 25
#define PUMP_PIN 33
#define PUMP_RECIRCUL_PIN 21
#define ASP_PIN 32

static const char *TAG = "primerversion";
/*==================[typedef]================================================*/

/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/
void vScreeningTask(void *pvParameters);


#endif   #ifndef INTERFACE_H