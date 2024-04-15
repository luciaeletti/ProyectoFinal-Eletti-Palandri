/** @file UserIO.c
 *  @brief It handle user's input-Output, including generic display and keypad.
 *
 *  UserIO is responsible for\n
 *  -xxx.\n
 *  -xxx.
 * @version		1.0
 * @date		11/02/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "Common.h"        /*RTOS types and structures*/
#include "UserIO.h"
#include "Display.h"
#include "Keypad.h"
#include "Recipe.h"

#include "Storer.h"
#include "Transceiver.h"

#include "Alarms.h"        /*Alarms data*/
#include "Environmental.h" /*Environmental data*/
#include "Descriptors.h"   /*Descriptors data*/


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
STATIC uint8_t count;
/*****************************************************************************
 * Private functions
 ****************************************************************************/

STATIC void vKeyListenerTask(void *pvParameters);

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief 		Screening thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void vScreeningTask(void *pvParameters) {


	if(xTaskCreate(vKeyListenerTask,"KeyListener",180, NULL, (tskIDLE_PRIORITY + 3),NULL))
	{
		#if defined (__DEBUG_ON)
		printf("KeyListener Task created successfully.. \n");
		#endif
	}

	xSemaphoreTake(xDataStoredSemaphore,0);

	while(1){

		xSemaphoreTake(xDataStoredSemaphore,portMAX_DELAY);/*'Take' the semaphore blocks the task until data arrive */

	DSP_WriteSD();
	DSP_WriteSignal();
	DSP_WriteEnvironmental();
	DSP_WriteAlarm();

	}

}

void UIO_InitDriver(void){

	DSP_open();
	KEY_open();
}


/**
 * @brief 		Refreshing thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */
void UIO_Ticks(void){

	count++;
	if(count>50){
	count=0;
	DSP_refresh();
	}
}

/*
 * @brief 		KeyListener thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */

STATIC void vKeyListenerTask(void *pvParameters){

	char key, row, column, character,enter;
	character='.';
	row=0;
	column=0;


	/*KeyListener wait for any keypad iterrupt*/
	xSemaphoreTake(xKeyListenerSemaphore,0);

	while(1){

	xSemaphoreTake(xKeyListenerSemaphore,portMAX_DELAY);/*'Take' the semaphore blocks the task until any key are pressed */

	key= KEY_read();
	if(key=='E')
	{
		if(NewSession)
		{
			data_recipe=NEW_RECIPE;
			cont_recipe=NSAMPLES+1;
			NewSession=false;
			DSP_ChangeWindow("Login");
		}

	}
	enter=true;
	switch(key){

	case 'U':	if(column==0 && row==0){enter=false; break;} /*no permitido*/
				if(column==0 && row>0){
					row--;
					character='/';
				}
				else{
					if(character>=46 && character<58) /* 46->'.' 47-> '/' 58->':'  */
					character++;
				}
				break;

	case 'D':	if(column==0 && row==5){enter=false; break;} /*no permitido*/
				if(column==0){
					row++;
					character='/';
				}
				else{
					if(character>46 && character<58)
						character--;
				}
				break;

	case 'R':	if(column>=0 && column<5)
				column++;
				character='/';
				break;

	case 'L':	if(column>0 && column<=5)
				column--;
				character='/';
				break;
	default:	enter=false;
	}

	if(enter){/*para evitar que reingrese.. cada vez que se presiona un boton se producen dos eventos*/

			if(column<5 && row<=5){
				PutCharacter(row, column, character);
			}
			if(column==5 && row<5){
				row++;
				column=0;
				character='/';
				PutCharacter(row, column, character);
				}
			if(column==5 && row==5){

				DSP_ChangeWindow("Confirm");
				row=6;
				column=2;
			}

			if(column==1 && row==6){

				row=5;
				column=4;
				character='/';
				PutCharacter(row, column, character);
				DSP_ChangeWindow("Login");
			}
			if(column==3 && row==6){
				DSP_SaveLogin();/*Write the recipe introduced by the user */
				row=0;
				column=0;
				character='/';
				PutCharacter(row, column, character);
				DSP_ChangeWindow("Home");
				NewSession=true;
				xSemaphoreGive(xNewSessionSemaphore);/*New session*/

			}

	vTaskDelay(200 / portTICK_RATE_MS);/* 200 milliseconds debounce delay. */

		}
	}
}
