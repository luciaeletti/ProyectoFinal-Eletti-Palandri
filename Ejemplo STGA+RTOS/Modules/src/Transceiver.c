
/** @file Transceiver.c
 *  @brief Transmits the records stored in the SD memory to the remote server by GPRS.
 *
 *  Transceiveris responsible for\n
 *  -.\n
 *  -.\n
 * @version		1.0
 * @date		08/04/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "Common.h"
#include "Transceiver.h"

#include "Modem.h"
#include "GPRS.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
enum
{
	OFF=0,
	ON
};

static char signal,i;
static char fName[22]="";
static unsigned char Filesnotsent=0;
static char ModemTimer;
static unsigned char count=0;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief 		Acquiring thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vTransmittingTask(void *pvParameters) {

	ModemTimer=ON;

	TRX_InitDriver();

	MOD_GetSignal(&signal);
	GPRS_SetSignal(signal);

	ModemTimer=OFF;

	while(1){
		vTaskDelay(360000 / portTICK_RATE_MS); /* delay for a period of 5 minutes=300000 ticks*/

		MOD_SearchFile(fName, &Filesnotsent);
		if(Filesnotsent>0){

			ModemTimer=ON;

			MOD_IsWorking();
			MOD_GetSignal(&signal);
			GPRS_SetSignal(signal);

			if(signal>10 && signal<90)
			{
				MOD_Init();

				MOD_OpenFTP("pcb.bioingenieria.edu.ar.advertis.dyndns.org","pcb","12345699!");
				MOD_SendFile("/pulverizadoras/004/",fName);

				MOD_LastFileSent();
				MOD_Close();

			}

			ModemTimer=OFF;

		}
	}/*end while(1)*/
}


/**
 * @brief 		Refreshing thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void TRX_Ticks(void){

if(ModemTimer==ON)
	{count++;
	if(count>10)
		{
		count=0;
		MOD_TicksInput();
		}
	}
}


char TRX_GetSignal(void){
	return signal;
}


void TRX_InitDriver(void){
	MOD_Open();
}

