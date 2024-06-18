/*
 * Store.h
 *
 *  Created on: 03/06/2024
 *      Author: Prototipado
 */
#ifndef STORE_H
#define STORE_H

/*==================[inclusions]=============================================*/
#include <string.h>
#include <stdio.h>
#include "conditions.h"
#include "ds3231.h"
#include "definitions.h"
/*==================[macros]=================================================*/
/*==================[typedef]================================================*/
/*==================[internal data declaration]==============================*/
#define OFFSET 255
#define MAX_ADRESS 4095
/*==================[internal functions declaration]=========================*/
/*==================[external functions declaration]=========================*/
void ReadString(const char *data, uint16_t saddres, uint16_t eadrres);
void ReadInt16(uint16_t data, uint16_t saddres, uint16_t eadrres);
void ReadInt32(uint32_t data, uint16_t saddres, uint16_t eadrres);
void StoreString(const char *data, uint16_t saddres, uint16_t eadrres);
void StoreInt16(uint16_t data, uint16_t saddres, uint16_t eadrres);
void StoreInt32(uint32_t data, uint16_t saddres, uint16_t eadrres);






#endif #ifndef STORE_H