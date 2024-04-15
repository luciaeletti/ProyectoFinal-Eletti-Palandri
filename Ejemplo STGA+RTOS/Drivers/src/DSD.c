/** @file DSD.c
 *  @brief It handle user's input-Output, including generic display and keypad.
 *
 *  UserIO is responsible for\n
 *  -xxx.\n
 *  -xxx.
 * @version		1.0
 * @date		29/03/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */


#include "DSD.h"
#include "SDcard.h"
#include "board.h"

#include "diskio.h"
#include "ff.h"

#include "stdio.h"
#include "string.h"

#include "Common.h"

DSTATUS stat;
DWORD p2;
WORD w1;
BYTE resul, b1;
FIL filedata;
FRESULT res;
static FATFS Fatfs[1];

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static const char SavedFile[12]="logfile.txt";

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
static char DSD_Ascii2Value(char value);
static void DSD_RTC_SetReady(void);
static Bool DSD_RTC_TestReady(void);
static Bool DSD_RTC_GetCurrentDate(RTC_TIME_T* t);

static char DSD_Ascii2Value(char value)
{
	char aux1=0;

	if(value < 58 && value > 47  ) //es un numero entre 0 y 9
	{
		aux1=value - 48;
		return aux1;
	}
		return aux1;
}
/**
 * @brief
 * @param
 * @return
 */

static Bool DSD_RTC_TestReady(void)
{
	char *found = 0;
	char buf_temp [15]="";

	taskENTER_CRITICAL();
		DSD_OpenFile("RTC.txt",DSD_READ);
		DSD_ReadFile(buf_temp,15);
		DSD_CloseFile();
	taskEXIT_CRITICAL();

    found = strstr(buf_temp, "YES");
	if (found){
		  return TRUE;
	  }
return FALSE;
}
/**
 * @brief
 * @param
 * @return
 */
static Bool DSD_RTC_GetCurrentDate(RTC_TIME_T* t)
{
	char *found = 0;
	char buf_temp [35]="";

	char year [4]="";
	char month [2]="";
	char Day [2]="";
	char hours[2]="";
	char minutes [2]="";
	char seconds [2]="";

	taskENTER_CRITICAL();
		DSD_OpenFile("RTC.txt",DSD_READ);
		DSD_MoveCursor(END);
		DSD_MoveCursor(-32);
		DSD_ReadFile(buf_temp,32);
		DSD_CloseFile();
	taskEXIT_CRITICAL();

    found = strstr(buf_temp, "Current_date"); /*Current_date:aaaa-mm-dd hh:mi:se*/
	if (found){
		strncpy(year,found+13,4);
		strncpy(month,found+18,2);
		strncpy(Day,found+21,2);
		strncpy(hours,found+24,2);
		strncpy(minutes,found+27,2);
		strncpy(seconds,found+30,2);

		t->time[RTC_TIMETYPE_YEAR]=DSD_String2Int(year,4);
		t->time[RTC_TIMETYPE_MONTH]=DSD_String2Int(month,2);
		t->time[RTC_TIMETYPE_DAYOFMONTH]=DSD_String2Int(Day,2);
		t->time[RTC_TIMETYPE_HOUR]=DSD_String2Int(hours,2);
		t->time[RTC_TIMETYPE_MINUTE]=DSD_String2Int(minutes,2);
		t->time[RTC_TIMETYPE_SECOND]=DSD_String2Int(seconds,2);

		return TRUE;
	  }
return FALSE;
}
/**
 * @brief
 * @param
 * @return
 */
static void DSD_RTC_SetReady(void)
{
	taskENTER_CRITICAL();
	DSD_OpenFile("RTC.txt",DSD_WRITE);
	DSD_WriteFile("RCT READY? YES");
	DSD_CloseFile();
	taskEXIT_CRITICAL();
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
uint16_t DSD_String2Int(char string [], char lenght)
{
	uint16_t res;
	int aux0,aux1,aux2,aux3;
	if(lenght>4)lenght=4;

	if(lenght==1){
		aux0=(int) DSD_Ascii2Value(string[0])*1;
		res=aux0;
	}

	if(lenght==2){
		aux0=(int) DSD_Ascii2Value(string[0])*10;
		aux1=(int) DSD_Ascii2Value(string[1])*1;
		res=aux0+aux1;
	}

	if(lenght==3){
		aux0=(int) DSD_Ascii2Value(string[0])*100;
		aux1=(int) DSD_Ascii2Value(string[1])*10;
		aux2=(int) DSD_Ascii2Value(string[2])*1;
		res=aux0+aux1+aux2;
	}
	if(lenght==4){
		aux0=(int) DSD_Ascii2Value(string[0])*1000;
		aux1=(int) DSD_Ascii2Value(string[1])*100;
		aux2=(int) DSD_Ascii2Value(string[2])*10;
		aux3=(int) DSD_Ascii2Value(string[3])*1;
		res=aux0+aux1+aux2+aux3;
	}
	return res;
}

Status DSD_GetSavedFiles(char out[]){
	unsigned char i;
	for(i=0;i<12;i++)
	{
		out[i]=SavedFile[i];
	}
	return SUCCESS;
}


/**
 * @brief
 * @param
 * @return
 */
void DSD_RTC_Init(void){

	if(DSD_RTC_TestReady()!=TRUE)
	{
		RTC_TIME_T today;

		if(DSD_RTC_GetCurrentDate(&today)!=TRUE)
		{
			today.time[RTC_TIMETYPE_YEAR]=2017;
			today.time[RTC_TIMETYPE_MONTH]=1;
			today.time[RTC_TIMETYPE_DAYOFMONTH]=1;
			today.time[RTC_TIMETYPE_HOUR]=0;
			today.time[RTC_TIMETYPE_MINUTE]=0;
			today.time[RTC_TIMETYPE_SECOND]=0;
		}

		Chip_RTC_Init(LPC_RTC);
		Chip_RTC_Enable(LPC_RTC, ENABLE);
		Chip_RTC_SetFullTime(LPC_RTC,&today);

		DSD_RTC_SetReady();
	}
}

/**
 * @brief
 * @param
 * @return
 */
Status DSD_SD_Init(void){

	SDCARD_T sd;
	stat = disk_initialize(0);

	if (stat & STA_NOINIT) {

		sd.status=SD_FAIL;
		SD_SetStatus(&sd);

		return ERROR;
#ifdef __DEBUG_ON
			printf("MMC: No Init\r\n");
#endif
		}

		if (stat & STA_NODISK) {

			sd.status=SD_FAIL;
			SD_SetStatus(&sd);

			return ERROR;
#ifdef __DEBUG_ON
			printf("MMC: No Disk\r\n");
#endif
		}

		if (stat != 0) {

			sd.status=SD_FAIL;
			SD_SetStatus(&sd);
			return ERROR;
		}

		resul = f_mount(0, &Fatfs[0]);
		if (resul != FR_OK) {

			sd.status=SD_FAIL;
			SD_SetStatus(&sd);

			return ERROR;
#ifdef __DEBUG_ON
			 printf( "Failed to mount SD %d \r\n");
#endif
		}

		sd.status=SD_OK;
		SD_SetStatus(&sd);

return  SUCCESS;
}


/**
 * @brief
 * @param
 * @return
 */
uint8_t DSD_GetStatus(void){

	return (uint8_t) stat;

}


/**
 * @brief
 * @param
 * @return
 */
void DSD_OpenFile(char *fname,DSD_MODE_T mode)
{

	if(mode==DSD_READ){
			res = f_open(&filedata, fname, FA_OPEN_ALWAYS|FA_READ);
			if(res != FR_OK){
	#ifdef __DEBUG_ON
			 		 printf("Cannot open file for reading\r\n");
	#endif
			 	 }
			else{
				#ifdef __DEBUG_ON
		 		 printf("open file for reading succefull \r\n");
				#endif
			}
		}
	else {//if SD_WRITE
			res = f_open(&filedata, fname, FA_OPEN_ALWAYS|FA_WRITE);
			if(res != FR_OK){
	#ifdef __DEBUG_ON
				printf("Cannot open file for writing \r\n");
	#endif
			 }
			else{
				#ifdef __DEBUG_ON
		 		 printf("open data file for writing succefull \r\n");
				#endif
			}
	}

}

/**
 * @brief
 * @param
 * @return
 */

void DSD_CloseFile(void)
{
	res = f_close(&filedata);

#ifdef __DEBUG_ON
	if(res != FR_OK)
		printf("Cannot close data file \r\n");
	else
		printf("data file close succefull\r\n");
#endif

}


/**
 * @brief
 * @param
 * @return
 */
Status DSD_ReadFile(char* line, unsigned int size){

	UINT read;

	taskENTER_CRITICAL();
	{
		res = f_read(&filedata, line, size,&read);
	}
	taskEXIT_CRITICAL();

	if(res) return ERROR;

	return SUCCESS;
}

/**
 * @brief
 * @param
 * @return
 */
void DSD_WriteFile(char* line)
{
	taskENTER_CRITICAL();
	{
		res = f_printf(&filedata,line);
	}
	taskEXIT_CRITICAL();


}

/**
 * @brief
 * @param
 * @return
 */
Status DSD_MoveCursor(long jump){

	if(jump==END)
	{
		res=f_lseek(&filedata, filedata.fsize); /*end of file*/
		if(res) return ERROR;
	}
	else
	{
		res=f_lseek(&filedata, f_tell(&filedata)+jump); /* x bytes jump*/
		if(res) return ERROR;
	}

	return SUCCESS;
}

/**
 * @brief
 * @param
 * @return
 */
void DSD_TimeStamp(void)
{
	RTC_TIME_T t;
	Chip_RTC_GetFullTime(LPC_RTC, &t);
	f_printf(&filedata,"%04d-%02d-%02d	%02d:%02d:%02d	",(int)t.time[RTC_TIMETYPE_YEAR],(int)t.time[RTC_TIMETYPE_MONTH],
			(int)t.time[RTC_TIMETYPE_DAYOFMONTH],(int)t.time[RTC_TIMETYPE_HOUR],(int)t.time[RTC_TIMETYPE_MINUTE],(int)t.time[RTC_TIMETYPE_SECOND]);
	f_printf(&filedata,"\r\n");
	f_printf(&filedata,"\r\n");

}

unsigned long DSD_FileSize(void)
{
	return filedata.fsize;
}
