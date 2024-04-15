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

#include "Descriptors.h" /*Sensors data*/

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
short RegisteredSensorCount=0;
short Stack_index=0;

DSC_DESCRIPTOR_T SensorsList[MAX_NUM_SNS];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
void ClearListItem(short index){

	SensorsList[index].frequency=0;
	SensorsList[index].resolution=0;
	SensorsList[index].state=0;
	SensorsList[index].mode=0;
	SensorsList[index].id=0;

}
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
Status DSC_AddSensorToList(DSC_DESCRIPTOR_T* pDescriptor){


	if(RegisteredSensorCount<MAX_NUM_SNS)
	{
		SensorsList[RegisteredSensorCount]=*pDescriptor;
		RegisteredSensorCount++;
		Stack_index=RegisteredSensorCount;
	}
	else
	{
		return ERROR;
	}
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
Status DSC_RemoveSensorFromList(DSC_DESCRIPTOR_T* pDescriptor){

	short i;

	for(i=0;i<MAX_NUM_SNS;i++){
		if(SensorsList[i].id==pDescriptor->id)
		{
            ClearListItem(i);
            RegisteredSensorCount--;
            Stack_index=RegisteredSensorCount;
		}
	}

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
Status DSC_ResetSensorsList(void){

	char i;

	for(i=0;i<MAX_NUM_SNS;i++){
		ClearListItem(i);
	}
	RegisteredSensorCount=0;
	Stack_index=RegisteredSensorCount;

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
Status DSC_GetSensorFromList(DSC_DESCRIPTOR_T* pDescriptor){

	if(Stack_index>0){
	*pDescriptor=SensorsList[Stack_index-1];
	Stack_index--;
	}

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
Status DSC_RestoreList(short value){

	Stack_index=value;

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
short DSC_GetSensorListLength(void){

	return RegisteredSensorCount;
}
