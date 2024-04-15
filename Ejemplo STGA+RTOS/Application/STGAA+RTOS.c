
/** @file STGAA+RTOS.c
 *  @brief Client program, manages the scheduler, launch all modules.
 *  @version		3.0.0
 *  @date		    14/01/2016
 *  @author		    Bioing. Martina Mauro
 *  @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "Acquirer.h"
#include "Analyzer.h"
#include "UserIO.h"
#include "Storer.h"
#include "Transceiver.h"
#include "Common.h"
#include "Board.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static unsigned long delay_InititScreen=4000000;

/*****************************************************************************
 * Private functions
 ****************************************************************************/
/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
}

/**
 * @brief	This routine generate time base for timed task.
 * @return	Nothing, function should not exit
 */
static void vTimerTask(void *pvParameters)
{
	while(1)
	{
		TRX_Ticks();/*time base for modem*/
		UIO_Ticks();/*time base for display*/
		vTaskDelay(10 / portTICK_RATE_MS);/* delay for a period of 10 milliseconds. */
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
/**
 * @brief	main routine for STGAA V3.0.0.
 * 			-Hardware initialization.
 * 			-Drivers initialization.
 * 			-Creation and schedule of tasks.
 *
 * @return	Nothing, function should not exit
 */
int main(void)
{
	prvSetupHardware();

	while(delay_InititScreen--);/*important delay dont remove it... wait for display*/

	Board_Init();		/*Hardware initialization*/
	STR_InitDriver();   /*DSD -Data Store Device- driver initialization */
	UIO_InitDriver();   /*Display driver initialization */

	COMM_CreateSemaphores();  /*Create semaphores before launch task*/

	/* Screening thread */
	xTaskCreate(vScreeningTask,  "Screening",  	 configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1),NULL);

	/* Acquiring thread */
	xTaskCreate(vAcquiringTask,  "Acquiring",	 configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1),NULL);

	/* Monitoring thread */
	xTaskCreate(vMonitoringTask,  "Monitoring",  configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1),NULL);

	/* Storing thread */
	xTaskCreate(vStoringTask,	  "Storing",	 configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1),NULL);

	/* Transmitting thread */
	xTaskCreate(vTransmittingTask,"Transmitting",configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1),NULL);

	/* Timing thread */
	xTaskCreate(vTimerTask,		  "Timer",		 configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1),NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}
