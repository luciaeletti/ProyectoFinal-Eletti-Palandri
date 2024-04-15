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
#include "Display.h"
#include "Bitmaps.h"

#include "Recipe.h"			/*data*/
#include "GPRS.h"			/*data*/
#include "Alarms.h"			/*data*/
#include "Descriptors.h" 	/*data*/
#include "Environmental.h"	/*data*/
#include "SDcard.h"			/*data*/

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define  V_OPEN 0x0F
#define  V_CLOSE 0xF0

static enum
{
	OFF=0,
	ON
};

enum{
	Previous=-1,
	Current,
	Next
};

unsigned char PrevY= 0;
unsigned char PrevX=0;
char PrevSim=' ';

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
static Status  DSP_CreateGUI(void){
	// Window "H&T"
	GUI_Window_Create ("Login", "NONE", WHITE, "LOGIN", NULL, NULL, NULL);
	GUI_Picture_Create("Login","Login_icons",  5,23, 48, 40,InicioBitmaps,BLACK,NULL);
	GUI_Text_Create   ("Login", "row1", 60, 14, WHITE, "", BLACK, "RECETA  :/....");
	GUI_Text_Create   ("Login", "row2", 60, 22, WHITE, "", BLACK, "PRODUCTO: ....");
	GUI_Text_Create   ("Login", "row3", 60, 30, WHITE, "", BLACK, "DOSIS   : ....");
	GUI_Text_Create   ("Login", "row4", 60, 38, WHITE, "", BLACK, "VOLUMEN : ....");
	GUI_Text_Create   ("Login", "row5", 60, 46, WHITE, "", BLACK, "APLICA  : ....");
	GUI_Text_Create   ("Login", "row6", 60, 54, WHITE, "", BLACK, "REGENTE : ....");


	GUI_Window_Create ("Confirm", "Login", WHITE, "LOGIN", NULL, NULL, NULL);
	GUI_Text_Create   ("Confirm", "Agree", 50, 25, WHITE, "", BLACK, "¿CONFIRMAR RECETA?");

	GUI_Window_Create( "Home", "NONE", WHITE, "STGAA v3.0", NULL, NULL, NULL );
	GUI_Picture_Create("Home","Signal_icons",  111, 0, 8,8,NULL,WHITE,NULL);
	GUI_Picture_Create("Home","SD_icons"    ,  120, 0, 8,8,NULL,WHITE,NULL);

	GUI_Picture_Create("Home","GPS_icons",  5, 18, 16, 16,GpsBitmaps,BLACK,NULL);
    GUI_Text_Create   ("Home", "Latitude",  20, 16, WHITE, "", BLACK,  "--*--'--.-\"Y");
    GUI_Text_Create   ("Home", "Longitude",  20, 24, WHITE, "", BLACK, "--*--'--.-\"Y");

    GUI_Picture_Create("Home","WIND_icons",  5, 47, 16,16,WindsBitmaps,BLACK,NULL);
    GUI_Text_Create   ("Home", "WindSpeed",  20, 40, WHITE, "", BLACK, "0.0 Km/h ");
    GUI_Text_Create   ("Home", "WindDirection",  20, 48, WHITE, "", BLACK, "NNE");

    GUI_Picture_Create("Home","FLOW_icons", 78, 18, 16,16,FlowBitmaps,BLACK,NULL);
    GUI_Text_Create   ("Home", "PumpState",  90, 16, WHITE, "", BLACK, "OFF");
    GUI_Text_Create   ("Home", "Flow",  90, 24, WHITE, "", BLACK, "0 L/m");

    GUI_Picture_Create("Home","TYH_icons", 78, 47, 16,16,TyhBitmaps,BLACK,NULL);
    GUI_Text_Create   ("Home", "Temperature",  90, 40, WHITE, "", BLACK, "0.0*C ");
    GUI_Text_Create   ("Home", "Humidity",  90, 48, WHITE, "", BLACK, "0 %HR ");

    GUI_Window_Draw ("Login");

	return SUCCESS;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
Status DSP_open( void){
	GUI_DefineHeapMalloc (pvPortMalloc);
	GUI_DefineHeapFree (vPortFree);
	GUI_StartLibrary ();
	DSP_CreateGUI();
	return SUCCESS;
}

Status DSP_WriteSignal(void){
	GPRS_SIGNAL_T mySignal;
	GPRS_GetSignal(&mySignal);

	TPicture* pPicture= (TPicture*) GUI_Window_SearchObject ("Signal_icons");

	if(mySignal==MARGINAL)
			pPicture->bitmap=Signal1Bitmaps;
	if(mySignal==POOR)
			pPicture->bitmap=Signal2Bitmaps;
	if(mySignal==GOOD)
			pPicture->bitmap=Signal3Bitmaps;
	if(mySignal==EXCELLENT)
			pPicture->bitmap=Signal4Bitmaps;

	return SUCCESS;
}

Status DSP_WriteSD(void){

	SDCARD_T tempSD;
	SD_GetStatus(&tempSD);

	TPicture* pPicture= (TPicture*) GUI_Window_SearchObject ("SD_icons");


	if(tempSD.status==SD_OK)
		pPicture->bitmap=SdOkBitmaps;
	if(tempSD.status==SD_FAIL)
		pPicture->bitmap=SdFailBitmaps;
	return SUCCESS;
}

Status DSP_WriteEnvironmental(void){
	ENV_ENVIRONMENTAL_T myEnvironmental;
	ENV_GetCurrentEnvironmental(&myEnvironmental);


	if(myEnvironmental.gps.lat_deegres==99)
	{
		TText* pText = (TText*) GUI_Window_SearchObject ("Latitude");
		sprintf(pText->text,"ERROR");
		pText = (TText*) GUI_Window_SearchObject ("Longitude");
		sprintf(pText->text,"ERROR");
	}
	else
	{
		TText* pText = (TText*) GUI_Window_SearchObject ("Latitude");
		sprintf(pText->text,"%d*%d'%3.1f\"S",(int)myEnvironmental.gps.lat_deegres,(int)myEnvironmental.gps.lat_minutes,myEnvironmental.gps.lat_seconds);  //"31*49'56.4\"S"

		pText = (TText*) GUI_Window_SearchObject ("Longitude");
		sprintf(pText->text,"%d*%d'%3.1f\"W",(int)myEnvironmental.gps.long_deegres,(int)myEnvironmental.gps.long_minutes,myEnvironmental.gps.long_seconds);// 60*31'20.2\"E

	}

	if(myEnvironmental.wind.speed==99)
	{
		TText* pText = (TText*) GUI_Window_SearchObject ("WindSpeed");
		sprintf(pText->text,"ERROR");
		pText = (TText*) GUI_Window_SearchObject ("WindDirection");
		sprintf(pText->text,"ERROR");
	}
	else
	{
		TText* pText = (TText*) GUI_Window_SearchObject ("WindSpeed");
		sprintf(pText->text,"%3.1f Km/h",myEnvironmental.wind.speed);  //9.1 Km/h

		pText = (TText*) GUI_Window_SearchObject ("WindDirection");
		sprintf(pText->text,"%d N",myEnvironmental.wind.direction);//NNE
	}

	if(myEnvironmental.flow.on_off==ON)
	{
		TText* pText = (TText*) GUI_Window_SearchObject ("PumpState");
		sprintf(pText->text,"ON");  //ON
		pText = (TText*) GUI_Window_SearchObject ("Flow");
		sprintf(pText->text,"%d L/m",(int)myEnvironmental.flow.value);//140 L/m
	}
	if(myEnvironmental.flow.on_off==OFF)
	{
		TText* pText = (TText*) GUI_Window_SearchObject ("PumpState");
		sprintf(pText->text,"OFF");  //ON
		pText = (TText*) GUI_Window_SearchObject ("Flow");
		sprintf(pText->text,"0 L/m");//140 L/m
	}
	if(myEnvironmental.flow.on_off==99)
	{
		TText* pText = (TText*) GUI_Window_SearchObject ("PumpState");
		sprintf(pText->text,"ERROR");  //OFF
		pText = (TText*) GUI_Window_SearchObject ("Flow");
		sprintf(pText->text,"ERROR");
	}

	if(myEnvironmental.tyh.temperature==99)
	{
		TText* pText = (TText*) GUI_Window_SearchObject ("Temperature");
		sprintf(pText->text,"ERROR");
		pText = (TText*) GUI_Window_SearchObject ("Humidity");
		sprintf(pText->text,"ERROR");

	}
	else
	{
		TText* pText = (TText*) GUI_Window_SearchObject ("Temperature");
		sprintf(pText->text,"%3.1f *C",myEnvironmental.tyh.temperature);  //24.5*C

		pText = (TText*) GUI_Window_SearchObject ("Humidity");
		sprintf(pText->text,"%3.1f %HR",myEnvironmental.tyh.humidity);//72 %HR
	}

	return SUCCESS;

}

Status DSP_WriteAlarm(void){
	ALARM_T Local_Alarm;
	TPicture* pPicture;
	ALARM_GetAlarm(&Local_Alarm);

	if(Local_Alarm.source==TEMPERATURE)
	{
		pPicture = (TPicture*) GUI_Window_SearchObject ("TYH_icons");
		pPicture->bitmap=alarmBitmaps;
		pPicture->xPos=73;
	}
	else
	{
		pPicture = (TPicture*) GUI_Window_SearchObject ("TYH_icons");
		pPicture->bitmap=TyhBitmaps;
		pPicture->xPos=78;
	}

	if(Local_Alarm.source==WINDSPEED)
	{
		pPicture = (TPicture*) GUI_Window_SearchObject ("WIND_icons");
		pPicture->bitmap=alarmBitmaps;
		pPicture->xPos=3;
	}
	else
	{
		pPicture = (TPicture*) GUI_Window_SearchObject ("WIND_icons");
		pPicture->bitmap=WindsBitmaps;
		pPicture->xPos=5;
	}

	if(Local_Alarm.source==POSITION)
	{
		pPicture = (TPicture*) GUI_Window_SearchObject ("GPS_icons");
		pPicture->bitmap=alarmBitmaps;
		pPicture->xPos=3;
	}
	else
	{
		pPicture = (TPicture*) GUI_Window_SearchObject ("GPS_icons");
		pPicture->bitmap=GpsBitmaps;
		pPicture->xPos=5;
	}

	if(Local_Alarm.source==FLOW)
	{
		pPicture = (TPicture*) GUI_Window_SearchObject ("FLOW_icons");
		pPicture->bitmap=alarmBitmaps;
		pPicture->xPos=73;
	}
	else
	{
		pPicture = (TPicture*) GUI_Window_SearchObject ("FLOW_icons");
		pPicture->bitmap=FlowBitmaps;
		pPicture->xPos=78;
	}

	return SUCCESS;
}


Status PutCharacter(unsigned char posX,unsigned char  posY, unsigned char simbol){

	TText* Textos[6]=
	{(TText*) GUI_Window_SearchObject ("row1") ,(TText*) GUI_Window_SearchObject ("row2"),
	 (TText*) GUI_Window_SearchObject ("row3") ,(TText*) GUI_Window_SearchObject ("row4"),
	 (TText*) GUI_Window_SearchObject ("row5") ,(TText*) GUI_Window_SearchObject ("row6")};

	char auxText[15]="";/*PRODUCTO: .... = 15*/
	int dir;
	unsigned char CurrY= posY;
	unsigned char CurrX=posX;
	unsigned char CurrSim=simbol;

	if(CurrY==0) /*Sie estoy en la columna 0 debo colocar el cursor*/
	{
		if(CurrX==0) /*se puede dar cuando voy subiendo en el menu*/
		{
			strcpy(auxText,Textos[CurrX+Current]->text);
			auxText[9]='/';
			strcpy(Textos[CurrX+Current]->text,auxText);

			strcpy(auxText,Textos[CurrX+Next]->text);
			auxText[9]=' ';
			strcpy(Textos[CurrX+Next]->text,auxText);

		}

		if(CurrX>0 && CurrX<5) /*cada vez que bajo en el menu*/
		{
			strcpy(auxText,Textos[CurrX+Previous]->text);
			auxText[9]=' ';

			if(auxText[13]=='/') auxText[13]=PrevSim;

			strcpy(Textos[CurrX+Previous]->text,auxText);

			strcpy(auxText,Textos[CurrX+Current]->text);
			auxText[9]='/';PrevSim=' ';
			strcpy(Textos[CurrX+Current]->text,auxText);

			strcpy(auxText,Textos[CurrX+Next]->text);
			auxText[9]=' ';
			strcpy(Textos[CurrX+Next]->text,auxText);
		}

		if(CurrX==5) /*ultima fila*/
		{
			strcpy(auxText,Textos[CurrX+Previous]->text);
			auxText[9]=' ';

			if(auxText[13]=='/') auxText[13]=PrevSim;

			strcpy(Textos[CurrX+Previous]->text,auxText);

			strcpy(auxText,Textos[CurrX+Current]->text);
			auxText[9]='/';PrevSim=' ';
			strcpy(Textos[CurrX+Current]->text,auxText);

		}
	}

	/*cambio de caracteres*/

    	dir=CurrY-PrevY;

    	if(dir>0)
    	{
        	strcpy(auxText,Textos[CurrX]->text);
    		if(auxText[9+Previous+CurrY]=='/') /*si no modifico el simbolo anterior*/
    		{auxText[9+Previous+CurrY]=PrevSim;}
    		strcpy(Textos[CurrX]->text,auxText);

    		strcpy(auxText,Textos[CurrX]->text);
        	PrevSim=auxText[9+CurrY];
        	auxText[9+CurrY]=CurrSim;
        	strcpy(Textos[CurrX]->text,auxText);
    	}
    	else
    	{
        	strcpy(auxText,Textos[CurrX]->text);
        	auxText[9+CurrY]=CurrSim;
        	strcpy(Textos[CurrX]->text,auxText);

        	strcpy(auxText,Textos[CurrX]->text);
    		if(auxText[9+Next+CurrY]=='/') /*si no modifico el simbolo siguiente*/
    		{auxText[9+Next+CurrY]=PrevSim;}
    		strcpy(Textos[CurrX]->text,auxText);
    	}

	PrevY=CurrY;
	PrevX=CurrX;

	return SUCCESS;
}

Status DSP_SaveLogin(void){

	char i=0;
	char* pStr[6];

	TText* Textos[6]={
		(TText*) GUI_Window_SearchObject ("row1") ,(TText*) GUI_Window_SearchObject ("row2"),
		(TText*) GUI_Window_SearchObject ("row3") ,(TText*) GUI_Window_SearchObject ("row4"),
		(TText*) GUI_Window_SearchObject ("row5") ,(TText*) GUI_Window_SearchObject ("row6")
	};

	RCP_Recipe_T myRecipe;

	if(myRecipe.receta[0]=='.') {return SUCCESS;}
	else
	{

		for(i=0;i<LOGIN_PARAMETERS_NUM;i++){
			pStr[i]=strchr(Textos[i]->text,':');
		}
		strncpy(myRecipe.receta   ,pStr[0]+2,4);
		strncpy(myRecipe.producto ,pStr[1]+2,4);
		strncpy(myRecipe.dosis    ,pStr[2]+2,4);
		strncpy(myRecipe.volumen  ,pStr[3]+2,4);
		strncpy(myRecipe.aplicador,pStr[4]+2,4);
		strncpy(myRecipe.regente  ,pStr[5]+2,4);

		RCP_AddRecipe(&myRecipe);
	}

	return SUCCESS;
}


Status DSP_ChangeWindow(char* name){
	GUI_Window_Draw(name);
return SUCCESS;
}

Status DSP_refresh(void){
	GUI_Window_RedrowActiveWindow();
	return SUCCESS;
}
