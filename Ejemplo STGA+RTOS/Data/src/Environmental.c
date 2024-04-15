/** @file EnvironmentalData.c
 *  @brief Persist environmental condition.
 *
 *  SensorsData is responsible for\n
 *  - \n
 *  - .
 * @version		1.0
 * @date		14/01/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "Environmental.h" /*Environmental data*/

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
ENV_ENVIRONMENTAL_T Current,Previous;

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
 * @brief 		Acquiring thread
 * @param[in] 	CANx point to LPC_CAN_TypeDef object, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	baudrate: is the baud rate value will be set
 * @see publicVar()
 * @return 		None
 */
Status ENV_SetCurrentEnvironmental(ENV_ENVIRONMENTAL_T* pEnvironment){

	Current=*pEnvironment;

return SUCCESS;
}

/**
 * @brief 		Acquiring thread
 * @param[in] 	CANx point to LPC_CAN_TypeDef object, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	baudrate: is the baud rate value will be set
 * @see publicVar()
 * @return 		None
 */
Status ENV_GetCurrentEnvironmental(ENV_ENVIRONMENTAL_T* pEnvironment){

	*pEnvironment=Current;

	return SUCCESS;
}

/**
 * @brief 		Acquiring thread
 * @param[in] 	CANx point to LPC_CAN_TypeDef object, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	baudrate: is the baud rate value will be set
 * @see publicVar()
 * @return 		None
 */
Status ENV_SetPreviousEnvironmental(ENV_ENVIRONMENTAL_T* pEnvironment){

	Previous=*pEnvironment;

return SUCCESS;
}

/**
 * @brief 		Acquiring thread
 * @param[in] 	CANx point to LPC_CAN_TypeDef object, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	baudrate: is the baud rate value will be set
 * @see publicVar()
 * @return 		None
 */
Status ENV_GetPreviousEnvironmental(ENV_ENVIRONMENTAL_T* pEnvironment){

	*pEnvironment=Previous;

	return SUCCESS;
}


/**
 * @brief 		Acquiring thread
 * @param[in] 	CANx point to LPC_CAN_TypeDef object, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	baudrate: is the baud rate value will be set
 * @see publicVar()
 * @return 		None
 */
Status ENV_ClearEnvironmental(void){

	Previous.flow.on_off=0;
	Previous.flow.value=0;
	Previous.gps.lat_deegres=0;
	Previous.gps.lat_minutes=0;
	Previous.gps.lat_seconds=0;
	Previous.gps.long_deegres=0;
	Previous.gps.long_minutes=0;
	Previous.gps.long_seconds=0;
	Previous.tyh.humidity=0;
	Previous.tyh.temperature=0;
	Previous.wind.direction=0;
	Previous.wind.speed=0;

	Current=Previous;

	return SUCCESS;
}
