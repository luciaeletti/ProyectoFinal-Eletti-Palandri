
#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#include "chip.h"


/**
 * CAN Port defines
 */
#define USE_CAN LPC_CAN2
#define CAN2_PORT2_PIN7_8
#define CAN_BIT_RATE 125000
#define CAN_EN_PORT 2
#define CAN_EN_PIN 6
#define CAN_STB_PORT 2
#define CAN_STB_PIN 5

/**
 * Keypad defines
 */
#define KEYPAD_PORT_ROWS 1
#define KEYPAD_PORT_COLUMN_0 0
#define KEYPAD_PORT_COLUMN_1 2

#define ROW_0 0
#define ROW_1 1
#define ROW_2 4
#define ROW_3 8
#define COLUMN_0 9
#define COLUMN_1 0

/*Note: EINT3 channel is shared with GPIO0/2 interrupts--User manual-Rev. 2 — 19 August 2010- PAG 74 of 840*/
#define GPIO_IRQ_HANDLER  			EINT3_IRQHandler/* GPIO interrupt IRQ function name */
#define GPIO_INTERRUPT_NVIC_NAME    EINT3_IRQn	/* GPIO interrupt NVIC interrupt name */

/**
 * SD Card defines
 */
#define SD_SSP LPC_SSP0

#define SD_DETECT_PORT 0
#define SD_CS_PORT 0
#define SD_DETECT_PIN 19
#define SD_CS_PIN 16


/**
 * LCD PWM contrast and brightness adjustment
 */
#define PORT_PWM_LCD 3
#define PIN_PWM_CONTRAST 25
#define PIN_PWM_BRIGHTNESS 26


/**
 * MODEM
 */
#define MODEM_USE_UART3
#define MODEM_UART  LPC_UART3
#define MODEM_RESET_PORT 2
#define MODEM_RESET_PIN 12

/**
 * @brief	Set up and initialize all required blocks and functions related to the board hardware.
 * @return	None
 */
void Board_Init(void);

#endif /* INC_BOARD_H_ */
