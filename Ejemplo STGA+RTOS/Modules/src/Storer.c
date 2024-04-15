/** @file Storer.c
 *  @brief Store environmental condition and alarms.
 *
 *  Storer is responsible for\n
 *  -\n
 *  -.
 * @version		1.0
 * @date		30/03/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "Common.h" /*RTOS types and structures*/
#include "Storer.h" /*Sensor types and structures*/
#include "GPRS.h"         /*data*/
#include "Alarms.h"         /*data*/
#include "Recipe.h"         /*data*/
#include "Environmental.h"  /*data*/
#include "DSD.h"
#include "string.h"
#include "stdio.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

static char LogFile[12]=" ";
static char FileName[22]=" ";
static uint16_t cont=NSAMPLES;

/*****************************************************************************
 * Private functions
 ****************************************************************************/
static void STR_NewFile(void);
static Status STR_RestoreFiles();
static Status STR_SaveData(void);
static void STR_WriteHead(void);
static void STR_WriteLabels(void);

/*****************************************************************************
 * Public functions
 ****************************************************************************/
/**
 * @brief
 * @param
 * @return
 */
void vStoringTask(void *pvParameters) {

	STR_RestoreFiles(); /*After reset*/
	xSemaphoreTake(xAnalysisDoneSemaphore,0);

	while(1){

		xSemaphoreTake(xAnalysisDoneSemaphore,portMAX_DELAY);/*'Take' the semaphore blocks the task until Analysis Done*/
		STR_SaveData();
		xSemaphoreGive(xDataStoredSemaphore); /*'Give' the semaphore to unblock the task*/
	}
}

void STR_InitDriver(void){

	DSD_SD_Init();
	DSD_RTC_Init();
	RCP_Init();

}

/**
 * @brief 		Refreshing thread
 * @param   	pvParameters is void pointer.
 * @return 		None
 */

static void STR_WriteRecipe(void){

	char BuffAux[100];
	RCP_Recipe_T myRecipe;
	RCP_GetRecipe(&myRecipe);

	int i_receta=DSD_String2Int   (myRecipe.receta,4);
	int i_producto=DSD_String2Int (myRecipe.producto,4);
	int i_dosis=DSD_String2Int    (myRecipe.dosis,4);
	int i_volumen=DSD_String2Int  (myRecipe.volumen,4);
	int i_aplicador=DSD_String2Int(myRecipe.aplicador,4);
	int i_regente=DSD_String2Int  (myRecipe.regente,4);
	int i_id=ID_SPRAYER;
	sprintf(BuffAux,"receta,%04d\r\nproducto,%04d\r\ndosis,%04d\r\nvolumen,%04d\r\naplicador,%04d\r\nregente,%04d\r\nid,%04d\r\n",i_receta,i_producto,i_dosis,i_volumen,i_aplicador,i_regente,i_id);
	DSD_WriteFile((char*)BuffAux);
	DSD_WriteFile("\r\n");

}

static void STR_WriteLabels(void){

	DSD_WriteFile("Temp	Hum	Vel	Dir	Lat	Lon	Caudal\r\n");
}


static void STR_WriteHead(void){

	DSD_WriteFile("++++++++++++++++++++++++++++++++++++++++++++\r\n");
	DSD_WriteFile("+               STGAA V3.0.0               +\r\n");
	DSD_WriteFile("+               ------------               +\r\n");
	DSD_WriteFile("+ EEA Paraná INTA - Lab.Prototipado FIUNER +\r\n");
	DSD_WriteFile("+   Todos los derechos reservados @2016    +\r\n");
	DSD_WriteFile("++++++++++++++++++++++++++++++++++++++++++++\r\n");
	DSD_WriteFile("\r\n");

}

static Status STR_SaveData(void){

	static char Buff_write[20];
	ENV_ENVIRONMENTAL_T myEnvironmental;
	ENV_GetCurrentEnvironmental(&myEnvironmental);

	if(cont_recipe<NSAMPLES)
	{
		cont_recipe++;

	}
	else{
		cont_recipe=0;
		STR_NewFile();
	}

	taskENTER_CRITICAL();
		DSD_OpenFile(FileName,DSD_WRITE);
		DSD_MoveCursor(END);
		sprintf((char*)Buff_write,"%3.1f	",myEnvironmental.tyh.temperature);  //24.5*C
		DSD_WriteFile((char*)Buff_write);
		sprintf((char*)Buff_write,"%3.1f	",myEnvironmental.tyh.humidity);//72 %HR
		DSD_WriteFile((char*)Buff_write);
		sprintf((char*)Buff_write,"%3.1f	",myEnvironmental.wind.speed);  //9.1 Km/h
		DSD_WriteFile((char*)Buff_write);
		sprintf((char*)Buff_write,"%d	",(int)myEnvironmental.wind.direction);//NNE
		DSD_WriteFile((char*)Buff_write);
		sprintf((char*)Buff_write,"%d°%d'%3.1f\"S	",(int)myEnvironmental.gps.lat_deegres,(int)myEnvironmental.gps.lat_minutes,myEnvironmental.gps.lat_seconds);  //"31*49'56.4\"S"
		DSD_WriteFile((char*)Buff_write);
		sprintf((char*)Buff_write,"%d°%d'%3.1f\"W	",(int)myEnvironmental.gps.long_deegres,(int)myEnvironmental.gps.long_minutes,myEnvironmental.gps.long_seconds);// 60*31'20.2\"E
		DSD_WriteFile((char*)Buff_write);
		sprintf((char*)Buff_write,"%d",(int)myEnvironmental.flow.value);//140 L/m
		DSD_WriteFile((char*)Buff_write);
	//	sprintf((char*)Buff_write,"%3.1f	%3.1f",myEnvironmental.gps.GPSSpeed,myEnvironmental.gps.GPSDirection);//140 L/m
	//	DSD_WriteFile((char*)Buff_write);
		DSD_WriteFile("\r\n");
		DSD_CloseFile();
	taskEXIT_CRITICAL();

	return SUCCESS;
}

/*Cierra el archivo para guardar lo escrito, abre un archivo nuevo con el sufijo del nombre incrementado*/
static void STR_NewFile(void){

	RCP_Recipe_T myRecipe;
	RCP_GetRecipe(&myRecipe);
	uint16_t NRecipe=DSD_String2Int(myRecipe.receta,4);

	DSD_GetSavedFiles(LogFile);

	if(data_recipe==FIRST_RECIPE) /*initialize files*/
	{
		data_recipe=0;
		sprintf(FileName,"%04dp_%04dr_%04dd.txt",(int)ID_SPRAYER,(int)NRecipe,(int)data_recipe);

		taskENTER_CRITICAL();
			DSD_OpenFile(LogFile,DSD_WRITE);
			DSD_WriteFile("LastFileSaved:");
			DSD_WriteFile(FileName);
			DSD_WriteFile("\r\n");
			DSD_WriteFile("LastFileSent:");
			DSD_WriteFile(FileName);
			DSD_CloseFile();
		taskEXIT_CRITICAL();
	}

	if(data_recipe==NEW_RECIPE) /*initialize files*/
	{
		data_recipe=0;
	}

	data_recipe++;

	sprintf(FileName,"%04dp_%04dr_%04dd.txt",(int)ID_SPRAYER,(int)NRecipe,(int)data_recipe);

	taskENTER_CRITICAL();
		DSD_OpenFile(FileName,DSD_WRITE);
		STR_WriteHead();
		STR_WriteRecipe();
		DSD_TimeStamp();
		STR_WriteLabels();
		DSD_CloseFile();
		DSD_OpenFile(LogFile,DSD_WRITE);
		DSD_WriteFile("LastFileSaved:");
		DSD_WriteFile(FileName);
		DSD_CloseFile();
	taskEXIT_CRITICAL();
}


/**
 * @brief
 * @param[in]
 * @see
 * @return
 */
static Status STR_RestoreFiles(){

	char BuffAux[40];
	char DataStr[5 ]="   ";

	char * pfound=NULL;
	DSD_GetSavedFiles(LogFile);

	DSD_OpenFile(LogFile,DSD_READ);
	DSD_ReadFile(BuffAux,36);
	DSD_CloseFile();

	pfound = strstr(BuffAux,"LastFileSaved:");
	if (pfound)
	{
		strncpy(FileName, (pfound+14),21); /* 0001p_0012r_0003d.txt */
		strncpy(DataStr,  (pfound+26),4);
		data_recipe=DSD_String2Int(DataStr,4);
		cont_recipe=NSAMPLES+1;
	}
	else
	{data_recipe=FIRST_RECIPE;
	cont_recipe=NSAMPLES+1;}

	return SUCCESS;
}
