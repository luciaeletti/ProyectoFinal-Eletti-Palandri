
/** @file GL865.c
 *  @brief (agregar comentario).
 *
 *  Sensor is responsible for\n
 *  -.\n
 *  -.
 * @version		1.0
 * @date		12/04/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "Board.h"
#include "GL865.h"
#include "stdio.h"
#include "string.h"
#include "Common.h"

#define BUF_LENGTH 127

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
uint16_t Timer;
uint8_t RX_buff[BUF_LENGTH];
uint8_t rxi=0;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static char AsciiToValue(char value)
	{/////AsciiToValue/////// Una funcion que nos devuelva el valor del char de entrada
		char aux1=0;
		if(value < 0x40) //es un numero le resto 0x30
		{
			aux1=value - 0x30;
			return aux1;
		}
		else
		{
			aux1= value -0x41+0x0A ; //es una letra, le resto 0x41 y le sumo 0x0A
			return aux1;
		}
	}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
static void GL865_SendByte(uint8_t Data){

	Chip_UART_SendByte(MODEM_UART,Data);
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
static void GL865_ClearBuff(void){

	uint8_t i=0;
	for(i=0;i<BUF_LENGTH;i++){
		RX_buff[i]=0;
	}
	rxi=0;
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
static uint8_t GL865_GetsTimeout(char *buf, uint16_t timeout) {
	uint8_t count = 0;
	char c;
	*buf = 0;
	Timer = timeout;
	while ( (Timer) && (count < (BUF_LENGTH - 1)) ){
		if(rxi>count){
			c = RX_buff[count];
			*buf++ = c;
			count++;
		}
	}
	if (count != 0){
		*buf = 0;
		count++;
		GL865_ClearBuff();
	}

	return count;
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
static Status GL865_RequestModem(const char *command, uint16_t timeout, Bool check, char *buf) {

	Status res;
	uint8_t count=0;
	char *found = 0;
	*buf = 0;

	if(!GL865_SendData(command, strlen(command))>0){/*if modem no send data, then rise an error*/
		res=ERROR;
		return res;
	}

	count = GL865_GetsTimeout(buf, timeout);

	#if defined (__DEBUG_ON)
	unsigned char i;
	printf("buf:");
	for(i=0;i<count;i++)
		printf("%c",buf[i]);
	#endif


	if (count) {
	  if (check) {
		  found = strstr(buf, "\r\nOK\r\n");
		  if (found){
			  res=SUCCESS;
		  }
		  else {
			  res=ERROR;
		  }
	  }
	  else {
		  res=SUCCESS;
	  }
	}
	else {
	  res=ERROR;
	}

	return res;
}


/*****************************************************************************
 * Public functions
 ****************************************************************************/
/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_IsWorking(void){

	Status res=ERROR;
	char buf[BUF_LENGTH];
	res= GL865_RequestModem("AT\r", 10, TRUE, buf);
	return res;
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_OperatorSelection(void){

	Status res=ERROR;
	char buf[BUF_LENGTH];
	res= GL865_RequestModem("AT+COPS?\r", 50, TRUE, buf);
	return res;
}


/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_Reset(void){

	Chip_GPIO_SetPinState(LPC_GPIO, MODEM_RESET_PORT, MODEM_RESET_PIN, true);
	Timer =3; /*300 mseg delay*/
	while (Timer>0){};
	Chip_GPIO_SetPinState(LPC_GPIO, MODEM_RESET_PORT, MODEM_RESET_PIN, false);
	Timer =10; /* 1 seg delay*/
	while (Timer>0){};
	return SUCCESS;

}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_Init(void){

	Status res=ERROR;
	char buf[BUF_LENGTH];
	GL865_RequestModem("AT\r", 10, TRUE, buf);
	GL865_RequestModem("AT&K=0\r", 10, TRUE, buf);
	GL865_RequestModem("AT+IPR=19200\r", 10, TRUE, buf);
	res=GL865_RequestModem("AT+CMEE=2\r", 10, TRUE, buf);

	return res;
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_CheckNetwork(void) {
  char buf[BUF_LENGTH];
  char result;
  Status res;

  #if defined (__DEBUG_ON)
  printf("checking network ...\r\n");
	#endif

  GL865_RequestModem("AT+CREG?\r", 10, FALSE, buf);


  result = buf[20];
  if ((result == '1')||(result == '5')) {
    res= SUCCESS;
  }
  else {
    res=ERROR;
  }

return res;
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_CheckGPRS(void) {
  char buf[BUF_LENGTH];
  char result;
  Status res;

/*
  	AT#GPRS?
	#GPRS: <status>
	where:
	<status>
	0 - PDP context #1 deactivated
	1 - PDP context #1 activated
	2 - PDP context #1 activation pending.
*/

  #if defined (__DEBUG_ON)
  printf("checking GPRS ...\r\n");
	#endif

  GL865_RequestModem("AT#GPRS?\r", 10, FALSE, buf);

  result = buf[18];
  if (result == '1') {
    res= SUCCESS;
  }
  else {
    res=ERROR;
  }

return res;
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_GetVersion(void) {
  char buf[BUF_LENGTH];
  Status res=ERROR;
#if defined (__DEBUG_ON)
  printf("version info ...\r\n");
#endif
	GL865_RequestModem("AT+GMI\r", 10, FALSE, buf);
	GL865_RequestModem("AT+GMM\r", 10, FALSE, buf);
	GL865_RequestModem("AT+GMR\r", 10, FALSE, buf);
	res=SUCCESS;

return res;
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_EscapeSequence(void) {

	char buf[BUF_LENGTH];
	 Status res=ERROR;

	GL865_GetsTimeout(buf, 10);
    GL865_SendData("+++",3);
	GL865_GetsTimeout(buf, 10);

	res=SUCCESS;

	return res;
}


/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
uint8_t GL865_GetSignal(void) {
  char buf[BUF_LENGTH];
  uint8_t res;
#if defined (__DEBUG_ON)
  printf("Get signal level...\r\n");
#endif
	GL865_RequestModem("AT+CSQ\r", 10, FALSE, buf);
	if(buf[16]==','){
	  res=AsciiToValue(buf[15]);}
	else{
		res=(10*AsciiToValue(buf[15])+AsciiToValue(buf[16]));
	}

return res;
}


/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_InitGPRS(void) {
  char buf[BUF_LENGTH];
  Status res=ERROR;
#if defined (__DEBUG_ON)
  printf("initializing GPRS ...\r\n");
#endif

 /*
  	GL865_RequestModem("AT#USERID=\"datos\"\r", 50, FALSE, buf);
	GL865_RequestModem("AT#PASSW=\"datos\"\r", 50, FALSE, buf);
	GL865_RequestModem("AT+CGDCONT=1,\"IP\",\"datos.personal.com\"\r", 10, TRUE, buf);
*/

   	GL865_RequestModem("AT#USERID=\"wap\"\r", 20, FALSE, buf);
	GL865_RequestModem("AT#PASSW=\"wap\"\r", 20, FALSE, buf);
	GL865_RequestModem("AT+CGDCONT=1,\"IP\",\"wap.gprs.unifon.com.ar\"\r", 20, TRUE, buf);

	res=SUCCESS;
	return res;

}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_EnableGPRS(void) {
  char buf[BUF_LENGTH];
  Status res=ERROR;
#if defined (__DEBUG_ON)
  printf("switching GPRS on ...\r\n");
#endif
  GL865_RequestModem("AT#GPRS=1\r", 20, FALSE, buf);

  res=SUCCESS;
  return res;
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_SetFileType(void) {
  char buf[BUF_LENGTH];
  Status res=ERROR;
#if defined (__DEBUG_ON)
  printf("setting ftp file transfer mode...\r\n");
#endif
  GL865_RequestModem("AT#FTPTYPE=0\r", 10, FALSE, buf);

  res=SUCCESS;
  return res;
}

Status GL865_GetMessage(void)
{
	char buf[BUF_LENGTH];
	Status res=ERROR;
	#if defined (__DEBUG_ON)
	printf("Get response from server...\r\n");
	#endif
	GL865_RequestModem("AT#FTPMSG\r", 10, FALSE, buf);

	res=SUCCESS;
	return res;
}

Status GL865_SetTimeout(void)
{
	char buf[BUF_LENGTH];
	Status res=ERROR;
	#if defined (__DEBUG_ON)
	printf("Settings of time-out...\r\n");
	#endif
	GL865_RequestModem("AT#FTPTO=200\r", 10, FALSE, buf);

	res=SUCCESS;
	return res;
}


/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_DisableGPRS(void) {
  char buf[BUF_LENGTH];
  Status res=ERROR;

#if defined (__DEBUG_ON)
printf("switching GPRS off ...\r\n");
#endif


  GL865_RequestModem("AT#GPRS=0\r", 10, FALSE, buf);

res=SUCCESS;
  return res;
}



/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_CloseSocket(void) {
  char buf[BUF_LENGTH];
  Status res=ERROR;

#if defined (__DEBUG_ON)
printf("Close socket...\r\n");
#endif

GL865_RequestModem("AT#FTPCLOSE\r", 50, FALSE, buf);

res=SUCCESS;
  return res;
}


/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_OpenFTP(const char *domain,const char *user,const char *password) {

	char buf[BUF_LENGTH];
	unsigned char i;
	for(i=0;i<BUF_LENGTH;i++)
		buf[i]=0;

	Status res=ERROR;
	char cmdbuf[BUF_LENGTH] = "AT#FTPOPEN=\"";

	#if defined (__DEBUG_ON)
	printf("opening socket ...\r\n");
	#endif

	strcat(cmdbuf, domain) ;
	strcat(cmdbuf,"\",\"");
	strcat(cmdbuf, user) ;
	strcat(cmdbuf,"\",\"");
	strcat(cmdbuf, password);
	strcat(cmdbuf,"\",1\r"); /*0-active mode(factory default) 1-passive mode*/


	if(GL865_RequestModem(cmdbuf, 50, FALSE, buf)==SUCCESS){

		#if defined (__DEBUG_ON)
		printf("socket open!\r\n");
		#endif
	}
	else{
		#if defined (__DEBUG_ON)
		printf("fail on socket open!\r\n");
		#endif
	}

	res=SUCCESS;
	return res;

}

Status GL865_ChangeFolderFTP(char* path) {

	char buf[BUF_LENGTH];
	unsigned char i;
	for(i=0;i<BUF_LENGTH;i++)
		buf[i]=0;

	#if defined (__DEBUG_ON)
	printf("Changing Folder.. \n");
	#endif

	Status res=ERROR;
	char cmdbuf[50] = "AT#FTPCWD=\"";
	strcat(cmdbuf, path);
	strcat(cmdbuf,"\"\r");

	if(GL865_RequestModem(cmdbuf, 50, FALSE, buf)==SUCCESS)res=SUCCESS;

	return res;
}

Status GL865_ConnectFTP(char* filename) {

	char buf[BUF_LENGTH];
	unsigned char i;
	for(i=0;i<BUF_LENGTH;i++)
		buf[i]=0;

	Status res=ERROR;

	#if defined (__DEBUG_ON)
	printf("Putting file.. \n");
	#endif

	char cmdbuf[50] = "AT#FTPPUT=\"";
	strcat(cmdbuf, filename);
	strcat(cmdbuf,"\"\r");
	GL865_RequestModem(cmdbuf, 50, FALSE, buf);

	if (strstr(buf, "CONNECT"))	res=SUCCESS;

	return res;
}


/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
Status GL865_SendSMS(char *number, char *message) {
  char buf[BUF_LENGTH];
  Status res=ERROR;
  const uint8_t CtrlZ =0x1A;
  char cmdbuf[30] = "AT+CMGS=\"";
#if defined (__DEBUG_ON)
  printf("sending SMS ...\r\n");
#endif
  GL865_RequestModem("AT+CMGF=1", 10, TRUE, buf);             // send text sms
  strcat(cmdbuf, number);
  strcat(cmdbuf, "\"");
  GL865_RequestModem(cmdbuf, 10, TRUE, buf);
  GL865_SendData(message, strlen(message));
  GL865_SendByte(CtrlZ);
  GL865_GetsTimeout(buf, 20);

  res=SUCCESS;
  return res;

}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
uint16_t GL865_SendData(const char* cmd,int size){
	uint16_t res=0;

	taskENTER_CRITICAL();
	res=Chip_UART_SendBlocking(MODEM_UART, cmd, size);
	taskEXIT_CRITICAL();

	return res;
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
uint16_t GL865_ReceiveData(char *buf) {

	return GL865_GetsTimeout (buf, 10);
}

/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */
void GL865_Timerproc(void){
	uint16_t n;

n = Timer;                  /* 100Hz decrement timer */
if (n) Timer = --n;

}



/*
 * @brief
 * @param[in]
 * @param[out]
 * @see
 * @return
 */

#ifdef MODEM_USE_UART0
void UART0_IRQHandler(void)
{
	uint8_t c;
	c=	Chip_UART_ReadByte(MODEM_UART);

	if(rxi<BUF_LENGTH){
	RX_buff[rxi]=c;
	rxi++;
	}
	else{
		rxi=0;
		}
}
#endif

#ifdef MODEM_USE_UART1
void UART1_IRQHandler(void)
{
	uint8_t c;
	c=	Chip_UART_ReadByte(MODEM_UART);

	if(rxi<BUF_LENGTH){
	RX_buff[rxi]=c;
	rxi++;
	}
	else{
		rxi=0;
		}
}

#endif

#ifdef MODEM_USE_UART2
void UART2_IRQHandler(void)
{
	uint8_t c;
	c=	Chip_UART_ReadByte(MODEM_UART);

	if(rxi<BUF_LENGTH){
	RX_buff[rxi]=c;
	rxi++;
	}
	else{
		rxi=0;
		}
}

#endif

#ifdef MODEM_USE_UART3
void UART3_IRQHandler(void)
{
	uint8_t c;
	c=	Chip_UART_ReadByte(MODEM_UART);

	if(rxi<BUF_LENGTH){
	RX_buff[rxi]=c;
	rxi++;
	}
	else{
		rxi=0;
		}
}

#endif


