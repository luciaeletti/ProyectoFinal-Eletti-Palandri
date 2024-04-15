/** @file SensorsData.c
 *  @brief Persist sensors registered list.
 *
 *  SensorsData is responsible for\n
 *  -Read data.\n
 *  -Send data.
 * @version		1.0
 * @date		14/01/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "GPRS.h" /*Sensors data*/

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
GPRS_SIGNAL_T mySignal;
char LastFileSent[20];

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
 * @param[in]
 * @see
 * @return
 */
Status GPRS_SetSignal(uint8_t CSQ){
	if(CSQ<9)
		mySignal=MARGINAL;
	if(CSQ>8 && CSQ<15)
		mySignal=POOR;
	if(CSQ>14 && CSQ<20)
		mySignal=GOOD;
	if(CSQ>20)
		mySignal=EXCELLENT;

	return SUCCESS;
}
/**
 * @brief
 * @param[in]
 * @see
 * @return
 */
Status GPRS_GetSignal(GPRS_SIGNAL_T* Signal){
	*Signal=mySignal;
	return SUCCESS;
}

/**
 * @brief
 * @param[in]
 * @see
 * @return
 */
Status GPRS_SetFileSent(char* FileName){

	strcpy(LastFileSent,FileName);
	return SUCCESS;
}

/**
 * @brief
 * @param[in]
 * @see
 * @return
 */
Status GPRS_GetFileSent(char* FileName){

	strcpy(FileName,LastFileSent);
	return SUCCESS;
}
