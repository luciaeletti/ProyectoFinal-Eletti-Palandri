/*
 * control.h
 *
 *  Created on: 03/06/2024
 *      Author: Prototipado
 */
#ifndef CONTROL_H
#define CONTROL_H

/*==================[inclusions]=============================================*/
#include <string.h>
#include <stdio.h>
#include "conditions.h"
#include "ds3231.h"
#include "definitions.h"

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions declaration]=========================*/
void vControlDuchaTask(void *pvParameters);
void vControlBombaTask(void *pvParameters);
void ManejoBombaDucha(bool state);
void vControlAutolavadoTask(void *pvParameters);
void vControlAspiradoraTask(void *pvParameters);
void ManejoAspiradora(bool state);
void vControlTiempoTask(void *pvParameters);
void ContarTiempo();
void GuardarDatosWF();





#endif #ifndef CONTROL_H