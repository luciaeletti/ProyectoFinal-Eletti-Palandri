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

/*==================[macros]=================================================*/
/*==================[typedef]================================================*/
/*==================[internal functions declaration]==========================*/
/*==================[external functions declaration]==========================*/
/*==================[internal functions definition]==========================*/
/*==================[external functions definition]==========================*/
void MemoryInit(){
    InitRom(&my_store);
}
//preguntar si es lo mismo usar lo q paso como parametro o si es preferible declarar adentro de la funcion
void ReadString(char *buffer, uint16_t saddress){
    size_t length = strlen(buffer);
    uint8_t data;
      for (size_t i = 0; i < length; i++) {
        esp_err_t err = ReadRom(my_store, saddress, &data);
            buffer[i] = (char)data;
        saddress++;
    }
}

uint16_t ReadInt16(uint16_t saddress){
    uint16_t value;
    uint8_t bytes[2]; 
    for (int i = 0; i < 2; i++) {
    esp_err_t err = ReadRom(my_store, saddress, &bytes[i]);
    saddress++;
    }
    value = ((uint16_t)bytes[0] << 8) | (uint16_t)bytes[1];
    return value;
}

uint32_t ReadInt32(uint16_t saddress){
    uint32_t value;
    uint8_t bytes[4]; 
    for (int i = 0; i < 4; i++) {
    esp_err_t err = ReadRom(my_store, saddress, &bytes[i]);
    saddress++;
    }
    value = ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) |
             ((uint32_t)bytes[2] << 8) | (uint32_t)bytes[3];
    return value;
}

void WriteString(char *data, uint16_t saddress){
    size_t length = strlen(data);
    for (size_t i = 0; i < length; i++) {
        char char_to_write = data[i];
        esp_err_t err = WriteRom(my_store, saddress, (uint8_t)char_to_write);
        saddress++;
    }
}

void WriteInt16(uint16_t value, uint16_t saddress){
    uint8_t bytes[2]; 
    bytes[0] = (value >> 8) & 0xFF;  
    bytes[1] = value & 0xFF;        
    for (int i = 0; i < 2; i++) {
    esp_err_t err = WriteRom(my_store, saddress, bytes[i]);
    saddress++;
    }
}

void WriteInt32(uint32_t value, uint16_t saddress){
    uint8_t bytes[4]; 
    bytes[0] = (value >> 24) & 0xFF; 
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;         
    for (int i = 0; i < 4; i++) {
    esp_err_t err = WriteRom(my_store, saddress, bytes[i]);
    saddress++;
    }
}