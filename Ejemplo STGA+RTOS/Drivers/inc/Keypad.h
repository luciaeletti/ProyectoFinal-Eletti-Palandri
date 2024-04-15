/** @file Keypad.h
 *  @brief (agregar comentario).
 *
 *  Keypad is responsible for\n
 *  -.\n
 *  -.
 * @version		1.0
 * @date		21/03/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "lpc_types.h"

#define NO_KEY  '\0'
#define ROWS  4 //four rows
#define COLS  2 //three columns

typedef struct {
    uint8_t rows;
    uint8_t columns;
} KeypadSize;

Status KEY_open( void);
Status KEY_close( void);
char   KEY_read( void);

#endif /* INC_KEYPAD_H_ */
