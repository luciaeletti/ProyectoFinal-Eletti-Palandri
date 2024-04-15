/** @file SDcard.c
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

#include "SDcard.h" /*Sensors data*/

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static SDCARD_T mySD;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief
 * @param
 * @return
 */
Status SD_SetStatus(SDCARD_T* sd){

	mySD.status=sd->status;
	return SUCCESS;
}

/**
 * @brief
 * @param
 * @return
 */
Status SD_GetStatus(SDCARD_T* sd){

	sd->status=mySD.status;
	return SUCCESS;
}

