
/** @file GL865.h
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


#ifndef GL865_H_
#define GL865_H_

#include "lpc_types.h"


Status GL865_Init(void);
Status GL865_Reset(void);
Status GL865_IsWorking(void);
Status GL865_OperatorSelection(void);

Status GL865_CheckNetwork(void);
Status GL865_CheckGPRS(void);
Status GL865_GetVersion(void);
Status GL865_EscapeSequence(void);
Status GL865_SetFileType(void);
Status GL865_GetMessage(void);
Status GL865_SetTimeout(void);
uint8_t GL865_GetSignal(void);

Status GL865_InitGPRS(void);
Status GL865_EnableGPRS(void);
Status GL865_DisableGPRS(void);
Status GL865_CloseSocket(void);
Status GL865_OpenFTP(const char *domain,const char *user,const char *password);
Status GL865_ConnectFTP(char* filename);
Status GL865_ChangeFolderFTP(char* path);
Status GL865_SendSMS(char *number, char *message);
uint16_t GL865_SendData(const char* cmd,int size);
uint16_t GL865_ReceiveData(char *buf);
void GL865_Timerproc(void);

#endif /* GL865_H_ */
