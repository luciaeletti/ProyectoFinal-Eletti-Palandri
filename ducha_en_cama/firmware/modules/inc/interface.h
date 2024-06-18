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
#include "definitions.h"
/*==================[macros]=================================================*/



static const char *TAG = "primerversion";

/*==================[typedef]================================================*/

/*==================[internal functions declaration]==========================*/
void menuInit();
void sub_menu(uint8_t select);
void sub_menu_ducha();
void sub_menu_configuracion();
void sub_menu_autolavado(); 
void delayMs(const TickType_t mSec);
void print_menu(const char *menus[], int num_menus, int selected);
void button_timer_callback(TimerHandle_t xTimer);
/*==================[external functions declaration]==========================*/
void vScreeningTask(void *pvParameters);


#endif   #ifndef INTERFACE_H