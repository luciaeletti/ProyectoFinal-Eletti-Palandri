/*
 * Display.h
 *
 *  Created on: 11/2/2016
 *      Author: Prototipado
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "lpc_types.h"
#include <string.h>
// FreeRTOS library includes
#include "FreeRTOS.h"
// GUI library includes
#include "GLCDgui.h"
#include "window.h"
#include "button.h"
#include "textSlider.h"
#include "text.h"
#include "picture.h"
#include "SystemFont5x7.h"
#include "GLCDguiEvent.h"
#include "object.h"


#include "GLCDColor.h"
#include "GLCDPrimitives.h"

#define LOGIN_PARAMETERS_NUM 6 /*cantidad de parametros que se cargan en el login*/

Status DSP_open( void);

Status PutCharacter(unsigned char posX,unsigned char  posY, unsigned char simbol);
Status DSP_WriteAlarm(void);
Status DSP_WriteSignal(void);
Status DSP_WriteSD(void);
Status DSP_WriteEnvironmental(void);

Status DSP_SaveLogin(void);
Status DSP_refresh(void);
Status DSP_ChangeWindow(char* name);

Status DSP_close( void);

#endif /* INC_DISPLAY_H_ */
