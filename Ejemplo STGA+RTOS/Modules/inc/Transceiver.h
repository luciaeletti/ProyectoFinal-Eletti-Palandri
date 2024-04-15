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


#ifndef INC_TRANSCEIVER_H_
#define INC_TRANSCEIVER_H_

void vTransmittingTask(void *pvParameters);
void TRX_Ticks(void);
void TRX_InitDriver(void);
char TRX_GetSignal(void);

#endif /* INC_TRANSCEIVER_H_ */
