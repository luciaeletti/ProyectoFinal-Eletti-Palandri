/** @file Analyzer.c
 *  @brief Evaluates the environmental condition,
 *  warning if the parameters are outside normal range.
 *
 *  Analyzer is responsible for\n
 *  -xxx.\n
 *  -xxx.
 * @version		1.0
 * @date		02/02/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "Common.h" /*RTOS types and structures*/
#include "Analyzer.h"

#include "Descriptors.h"
#include "Alarms.h"			/*Alarms data*/
#include "Environmental.h" 	/*Environmental data*/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
STATIC Status Evaluate_Ranges(void);

/*****************************************************************************
 * Public functions
 ****************************************************************************/
/**
 * @brief 		Monitoring thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vMonitoringTask(void *pvParameters) {

	xSemaphoreTake(xEnviromentFullSemaphore,0);

	#if defined (__DEBUG_ON)
	printf("Monitoring: blocked until data arrive..\n");
	#endif

	while(1){

		xSemaphoreTake(xEnviromentFullSemaphore,portMAX_DELAY);/*'Take' the semaphore blocks the task until data arrive */
		Evaluate_Ranges();/*read environmental condition and evaluate normal range*/
		xSemaphoreGive(xAnalysisDoneSemaphore); /*'Give' the semaphore to unblock the task*/
	}

}

/**
 * @brief 		Evaluate_Ranges
 * @param[in] 	CANx point to LPC_CAN_TypeDef object, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	baudrate: is the baud rate value will be set
 * @see publicVar()
 * @return 		None
 */
STATIC Status Evaluate_Ranges(void){
	ENV_ENVIRONMENTAL_T myEnvironment;
	DSC_DESCRIPTOR_T myDescriptor;
	ALARM_T myAlarm;
	ENV_GetCurrentEnvironmental(&myEnvironment);

	uint8_t listSize=DSC_GetSensorListLength();
	uint8_t i;

	ALARM_ResetAlarm();

	for(i=0;i<listSize;i++)
	{
		DSC_GetSensorFromList(&myDescriptor);

		switch((char)myDescriptor.id)
		{
		case ID_GPS: 	if(myEnvironment.gps.lat_deegres>LAT_MAX || myEnvironment.gps.lat_deegres<LAT_MIN)
						{
							myAlarm.source=POSITION;
							myAlarm.scale=MEDIUM;
						}
						if(myEnvironment.gps.long_deegres>LON_MAX || myEnvironment.gps.long_deegres<LON_MIN)
						{
							myAlarm.source=POSITION;
							myAlarm.scale=HIGH;
						}
						ALARM_SetAlarm(&myAlarm);
						break;

		case ID_ANEMOMETER: 	if(myEnvironment.wind.speed>WSPEED_MAX_1)
								{
									myAlarm.source=WINDSPEED;
									myAlarm.scale=LOW;

									if(myEnvironment.wind.speed>WSPEED_MAX_3)myAlarm.scale=HIGH;
									if(myEnvironment.wind.speed>WSPEED_MAX_2)myAlarm.scale=MEDIUM;
									ALARM_SetAlarm(&myAlarm);
								}
									break;

		case ID_THERMOHYGROMETER: 	if(myEnvironment.tyh.temperature>TEMP_MAX_1)
									{
										myAlarm.source=TEMPERATURE;
										myAlarm.scale=LOW;

										if(myEnvironment.tyh.temperature>TEMP_MAX_3)myAlarm.scale=HIGH;
										if(myEnvironment.tyh.temperature>TEMP_MAX_2)myAlarm.scale=MEDIUM;
										ALARM_SetAlarm(&myAlarm);
									}
									break;

		case ID_FLOWMETER: 	if(myEnvironment.flow.value>FLOW_MAX_1)
							{
								myAlarm.source=FLOW;
								myAlarm.scale=LOW;

								if(myEnvironment.flow.value>FLOW_MAX_3)myAlarm.scale=HIGH;
								if(myEnvironment.flow.value>FLOW_MAX_2)myAlarm.scale=MEDIUM;
								ALARM_SetAlarm(&myAlarm);
							}
							break;
		}
	}

	return SUCCESS;
}/*end function.*/
