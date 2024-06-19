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
#include "at24c.h"
#include "definitions.h"
/*==================[macros]=================================================*/
#define OFFSET 255
#define MAX_ADRESS 4095
/*==================[typedef]================================================*/
/*==================[internal data declaration]==============================*/
EEPROM_t *my_store;
/*==================[internal functions declaration]=========================*/
/*==================[external functions declaration]=========================*/
void MemoryInit();
void ReadString(char *buffer, uint16_t sadrress);
uint16_t ReadInt16(uint16_t saddres);
uint32_t ReadInt32(uint16_t saddres);
void WriteString(char *data, uint16_t saddress);
void WriteInt16(uint16_t data, uint16_t saddress);
void WriteInt32(uint32_t data, uint16_t saddress);






#endif #ifndef STORE_H