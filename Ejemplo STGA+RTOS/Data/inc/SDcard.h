/** @file SDcard.h
 *  @brief
 *
 *  SDcard is responsible for\n
 *  -xxx.\n
 *  -xxx.
 * @version		1.0
 * @date		14/07/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_


#include "lpc_types.h"


/**
 * @brief Flowmeter definitions
 */
typedef struct{
	char status;
}SDCARD_T;

enum
{
	SD_OK=0,
	SD_FAIL
};

Status SD_SetStatus(SDCARD_T* sd);
Status SD_GetStatus(SDCARD_T* sd);

#endif /* INC_SDCARD_H_ */
