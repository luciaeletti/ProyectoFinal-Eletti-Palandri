/** @file Modem.c
 *  @brief Transmits the records stored in the SD memory to the remote server by GPRS.
 *
 *  Modem responsible for\n
 *  -.\n
 *  -.\n
 * @version		1.0
 * @date		29/03/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "Common.h"
#include "Modem.h"
#include "GL865.h"
#include "DSD.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define Buff_RX_Size 255
static char LogFile[12]="";
static char LastFileSent[22]="";
/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief
 * @param
 * @return
 */
void MOD_TicksInput(void){

	GL865_Timerproc();
}

/**
 * @brief
 * @param
 * @return
 */
Status MOD_Open(void){

	Status res=ERROR;

	if(GL865_Init()==SUCCESS){

		//GL865_GetVersion();

			#if defined (__DEBUG_ON)
			printf("Modem Initialized: OK...\r\n");
			#endif

	}
	else{
			#if defined (__DEBUG_ON)
				printf("Modem Initialized: OK...\r\n");
			#endif
	}

return res;

}

/**
 * @brief
 * @param
 * @return
 */
Status MOD_IsWorking(void)
{
if(GL865_IsWorking()==ERROR) GL865_Reset();

return SUCCESS;
}

/**
 * @brief
 * @param
 * @return
 */
Status MOD_Init(void)
{
	GL865_CheckNetwork();

	while(GL865_CheckGPRS()==ERROR)
	{
	GL865_InitGPRS();
	GL865_EnableGPRS();
	}

	return SUCCESS;
}

/**
 * @brief
 * @param
 * @return
 */
void MOD_GetSignal(char *signal){
	*signal=GL865_GetSignal();
}

/**
 * @brief
 * @param
 * @return
 */
void MOD_GetVersion(char *version){
	GL865_GetVersion();
}


/**
 * @brief
 * @param
 * @return
 */
Status MOD_OpenFTP(const char *domain,const char *user,const char *password)
{
	Status res=ERROR;
	res=GL865_OpenFTP(domain,user,password);/*AT#FTPOPEN=?*/
	GL865_SetFileType();
	return res;
}


/**
 * @brief
 * @param
 * @return
 */
void MOD_Login(char* credentials){

}

/**
 * @brief
 * @param
 * @return
 * note searches for files with the same number that were not sent.
 */
Status MOD_SearchFile(char * str, unsigned char* diff)
{
	char BuffAux[75]="";
	char DataStr[5]="";
	char LastSent[21]="";

	char * pfound=NULL;
	unsigned char SavedFiles,SentFiles;
	*diff=0;

	DSD_GetSavedFiles(LogFile);
	DSD_OpenFile(LogFile,DSD_READ);
	DSD_ReadFile(BuffAux,75);
	DSD_CloseFile();

	pfound = strstr(BuffAux,"LastFileSaved:");
	if (pfound)
	{
		strncpy(DataStr, (pfound+26),4); /* "LastFileSaved:"=14 .. "0001p_0012r_"=12 */
		SavedFiles=(DSD_String2Int(DataStr,4))-1; //not send current file
	}

	pfound = strstr(BuffAux,"LastFileSent:");
	if (pfound)
	{
		strncpy(LastSent, (pfound+13),12); /* "LastFileSent:"=13 .. "0001p_0012r_"=12 */
		strncpy(DataStr, (pfound+25),4);
		SentFiles=DSD_String2Int(DataStr,4);
	}

	*diff=SavedFiles-SentFiles;

	sprintf(DataStr,"%04dd.txt",(SentFiles+1));
	strcat(LastSent,DataStr);
	strncpy(str,LastSent,21);/*"0001p_0012r_0001.txt"=20 */
	strncpy(LastFileSent,LastSent,21);

	return SUCCESS;
}


/**
 * @brief
 * @param
 * @return
 * note searches for files with the same number that were not sent.
 */
Status MOD_LastFileSent(void)
{
	DSD_GetSavedFiles(LogFile);
	DSD_OpenFile(LogFile,DSD_WRITE);
	DSD_MoveCursor(END);
	DSD_MoveCursor(-34);  /*LastFileSent:0001p_0012r_0003d.txt*/
	DSD_WriteFile("LastFileSent:");
	DSD_WriteFile(LastFileSent);
	DSD_CloseFile();

	return SUCCESS;
}


/**
 * @brief
 * @param
 * @return
 */
Status MOD_SendFile(char * path, char * str){


	NewSession=false;/*desactiva la adquisicion*/

	char fname[22]="";

	char mybuff[Buff_RX_Size];
	unsigned char i;
	for(i=0;i<Buff_RX_Size;i++)
	mybuff[i]=0;

	unsigned int index=0;
	strncpy(fname,str,21);

	GL865_ChangeFolderFTP(path);
	GL865_ConnectFTP(fname);


	DSD_CloseFile();
	DSD_OpenFile(fname,DSD_READ);

	unsigned long size=DSD_FileSize();
	unsigned int loop=size/Buff_RX_Size;
	unsigned int rest=size%Buff_RX_Size;

	#if defined (__DEBUG_ON)
	printf("Sending file.. \n");
	#endif

	while(index<loop)
	{
		DSD_ReadFile(mybuff,Buff_RX_Size);
		GL865_SendData(mybuff,Buff_RX_Size);
		index++;
	}

	DSD_ReadFile(mybuff,rest);
	GL865_SendData(mybuff,rest);
	DSD_CloseFile();

	GL865_EscapeSequence();

	NewSession=true;
	xSemaphoreGive(xNewSessionSemaphore);

	return SUCCESS;
}

/**
 * @brief
 * @param
 * @return
 */
void MOD_Close(void){
	GL865_CloseSocket();
	//GL865_DisableGPRS();
}
