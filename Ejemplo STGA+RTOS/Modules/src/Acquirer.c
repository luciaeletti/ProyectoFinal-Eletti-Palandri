/** @file Acquirer.c
 *  @brief Acquires signals from sensors.
 *
 *  Acquirer is responsible for\n
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

#include "Common.h" /*RTOS types and structures*/
#include "Acquirer.h"

#include "Sensor.h" /*Sensor types and structures*/
#include "Descriptors.h" /*Sensors data*/
#include "Environmental.h" /*Environmental data*/
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
CAN_MSG_T RcvMsgBuf;

char ReceivedCount;
char sensors_currently_registered;

/*****************************************************************************
 * Private functions
 ****************************************************************************/
static Status AskForData(void);
static Status AskForSensors(char id, char action);
STATIC Status ClearInactiveSensors(void);

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief 		Acquiring thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vAcquiringTask(void *pvParameters) {

	if(xTaskCreate(vSnsHandlerTask,"SnsHandler",128, NULL, (tskIDLE_PRIORITY + 2),NULL)==pdTRUE)
	{
		#if defined (__DEBUG_ON)
		printf("SnsHandler Task created successfully.. \n");
		#endif
	}

	xSemaphoreTake(xNewSessionSemaphore,0);
	NewSession=false;

	while(1){

		if(!NewSession)	xSemaphoreTake(xNewSessionSemaphore,portMAX_DELAY);/*'Take' the semaphore blocks the task until data arrive */
		AskForData(); /*lee lista de sensores activos y solicita datos*/
		vTaskDelay(4000 / portTICK_RATE_MS);/* delay for a period of 5 seconds. */
	}

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
STATIC Status AskForData(void){

	DSC_ResetSensorsList();              /*The SENSOR list must be updated.*/

	AskForSensors(ID_CPU, RECEIVE_CFG); /*Generate sensors list*/

	if(DSC_GetSensorListLength()!=FALSE)
	{
		AskForSensors(ID_CPU, RECEIVE_DATA); /*Ask for new data*/
		ClearInactiveSensors();
		/*'Give' the semaphore to unblock the task*/
		xSemaphoreGive(xEnviromentFullSemaphore);
	}
	else
	{
		#if defined (__DEBUG_ON)
		printf("ERROR sensors don't respond.. \n");
		#endif
		return ERROR;
	}

	return SUCCESS;
}


/**
 * @brief
 * @param
 * @return
 */
STATIC Status AskForSensors(char id, char action){

SNS_MSG_T myOutMsg;

myOutMsg.id= id;
myOutMsg.data[0]=action;

SNS_write(&myOutMsg);

vTaskDelay(1000 / portTICK_RATE_MS);/* delay for a period of 1 seconds..waiting for sensors. */

return SUCCESS;
}

STATIC Status ClearInactiveSensors(void)
{
	uint8_t list=0;
	uint8_t active_sensor=0;
	ENV_ENVIRONMENTAL_T myData;
	DSC_DESCRIPTOR_T mySensor;

	ENV_GetCurrentEnvironmental(&myData);

	list=DSC_GetSensorListLength();
	while(list--)
	{
		DSC_GetSensorFromList(&mySensor);

		if(mySensor.id==ID_GPS) active_sensor=active_sensor|0x01;
		if(mySensor.id==ID_ANEMOMETER)active_sensor=active_sensor|0x02;
		if(mySensor.id==ID_THERMOHYGROMETER)active_sensor=active_sensor|0x04;
		if(mySensor.id==ID_FLOWMETER)active_sensor=active_sensor|0x08;
	}
	DSC_RestoreList(DSC_GetSensorListLength());/*restore the index of list*/

	if(!(active_sensor&0x01))
	{
		 myData.gps.lat_deegres=99;
		 myData.gps.lat_minutes=99;
		 myData.gps.lat_seconds=99;
		 myData.gps.long_deegres=99;
		 myData.gps.long_minutes=99;
		 myData.gps.long_seconds=99;
	}
	if(!(active_sensor&0x02))
	{	myData.wind.direction=99;
		myData.wind.direction=99;
	}
	if(!(active_sensor&0x04))
	{	myData.tyh.temperature=99;
		myData.tyh.humidity=99;
	}
	if(!(active_sensor&0x08))
	{   myData.flow.on_off=99;
		myData.flow.value=99;}

	ENV_SetCurrentEnvironmental(&myData);

	return SUCCESS;
}
