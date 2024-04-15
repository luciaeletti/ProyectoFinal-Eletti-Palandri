/** @file Sensor.c
 *  @brief (agregar comentario).
 *
 *  Sensor is responsible for\n
 *  -.\n
 *  -.
 * @version		1.0
 * @date		20/01/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "Common.h"
#include "Board.h"
#include "Sensor.h"
#include "Descriptors.h" /*Sensors data*/
#include "Environmental.h" /*Environmental data*/

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

static enum
{
	OFF=0,
	ON=1,
	OPEN=0x0F,
	CLOSE=0xF0
};

CAN_MSG_T RcvMsgBuf;
char ReceivedDataCount =0;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/**
 * @brief 		ACQ_Acquiring thread
 * @param[in] 	CANx point to LPC_CAN_TypeDef object, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	baudrate: is the baud rate value will be set
 * @see publicVar()
 * @return 		None
 */
STATIC Status SNS_DataUnwrap(CAN_MSG_T * pReceivedMsg, ENV_ENVIRONMENTAL_T *  pEnvironmental){

/*CAN id_standard has 11 bits, should be read according to the format xxxyyyyzzzz.
 * xxx: Unused; yyyy:identifier; zzzz:command; */

	static float f_aux;
	static unsigned short i_aux;

	switch(pReceivedMsg->ID&0x07)
	{
	case ID_GPS:
					  pEnvironmental->gps.lat_deegres=pReceivedMsg->Data[0];
					  pEnvironmental->gps.lat_minutes=pReceivedMsg->Data[1];
					  pEnvironmental->gps.lat_seconds=((pReceivedMsg->Data[2])*256 +(pReceivedMsg->Data[3]))*0.006;
					  pEnvironmental->gps.long_deegres=pReceivedMsg->Data[4];
					  pEnvironmental->gps.long_minutes=pReceivedMsg->Data[5];
					  pEnvironmental->gps.long_seconds=((pReceivedMsg->Data[6])*256 +(pReceivedMsg->Data[7]))*0.006;
					  break;

	case ID_ANEMOMETER:

					 pEnvironmental->wind.speed=((pReceivedMsg->Data[1])*256 +(pReceivedMsg->Data[0]))*0.01;
					 pEnvironmental->wind.direction=((pReceivedMsg->Data[3])*256 +(pReceivedMsg->Data[2]));
        			 break;

	case ID_THERMOHYGROMETER:
					  pEnvironmental->tyh.temperature=((pReceivedMsg->Data[0])*256 +(pReceivedMsg->Data[1]))*0.01;
					  pEnvironmental->tyh.humidity=((pReceivedMsg->Data[2])*256 +(pReceivedMsg->Data[3]))*0.01;
					  break;
	case ID_FLOWMETER:

					  if(pReceivedMsg->Data[0]==OPEN)
					  {
						  pEnvironmental->flow.on_off=ON;

						  i_aux=((pReceivedMsg->Data[2])*256 +(pReceivedMsg->Data[1]));
						  if(i_aux>=0 && i_aux<250)
						  {
							  pEnvironmental->flow.value=i_aux;
						  }
					  }
					  else
					  {
						  pEnvironmental->flow.on_off=OFF;
						  pEnvironmental->flow.value=OFF;
					  }
				  break;

	default :	 return ERROR;
	}


	return SUCCESS;

}

STATIC Status SNS_DescriptorUnwrap(CAN_MSG_T * pReceivedMsg, DSC_DESCRIPTOR_T *  pDescriptor){

	pDescriptor->id=(pReceivedMsg->ID&0x07);
	pDescriptor->frequency=pReceivedMsg->Data[0];
	pDescriptor->mode=pReceivedMsg->Data[1];
	pDescriptor->resolution=pReceivedMsg->Data[2];
    pDescriptor->state=pReceivedMsg->Data[3];

    return SUCCESS;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

Status SNS_read (SNS_MSG_T *  InMsg){

	return  SUCCESS;
}

Status SNS_write(SNS_MSG_T *  myOutMsg){

	CAN_BUFFER_ID_T   TxBuf;
	CAN_MSG_T SendMsgBuf;

	SendMsgBuf.ID=myOutMsg->id;
	SendMsgBuf.DLC=CAN_MSG_MAX_DATA_LEN;
	SendMsgBuf.Type=0;

	uint8_t i;
	for(i=0;i<CAN_MSG_MAX_DATA_LEN;i++)
		SendMsgBuf.Data[i]=myOutMsg->data[i];

	taskENTER_CRITICAL();
	{
	TxBuf = Chip_CAN_GetFreeTxBuf(USE_CAN);
	Chip_CAN_Send(USE_CAN, TxBuf, &SendMsgBuf);
	}
	taskEXIT_CRITICAL();
	while ((Chip_CAN_GetStatus(USE_CAN) & CAN_SR_TCS(TxBuf)) == 0) {}

	return  SUCCESS;
}

void CAN_IRQHandler( void )
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	uint32_t IntStatus;

	IntStatus = Chip_CAN_GetIntStatus(USE_CAN);

	if (IntStatus & CAN_ICR_RI)
	{	/* New Message came */
		Chip_CAN_Receive(USE_CAN, &RcvMsgBuf);
		/*'Give' the semaphore to unblock the handler*/
		xSemaphoreGiveFromISR(xReceivedMsgSemaphore,&xHigherPriorityTaskWoken);
	}
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void vSnsHandlerTask( void *pvParameters ){

	DSC_DESCRIPTOR_T myDescriptor;
	ENV_ENVIRONMENTAL_T myEnvironmental;

	xSemaphoreTake(xReceivedMsgSemaphore,0);

	while(1){
		/*'Take' the semaphore blocks the handler until an interrupt arrive */
		xSemaphoreTake(xReceivedMsgSemaphore,portMAX_DELAY);

		if(((RcvMsgBuf.ID&0x18)>>3)==RECEIVE_CFG)
		{
				SNS_DescriptorUnwrap(&RcvMsgBuf,&myDescriptor);
				DSC_AddSensorToList(&myDescriptor);
		}
		else
		{
				SNS_DataUnwrap(&RcvMsgBuf,&myEnvironmental);
				ReceivedDataCount++;
				if(ReceivedDataCount>=DSC_GetSensorListLength())
				{
					ReceivedDataCount=0;
					ENV_SetCurrentEnvironmental(&myEnvironmental);
				}
		}

	}
}
