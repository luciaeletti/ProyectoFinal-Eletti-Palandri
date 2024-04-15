
#include "board.h"
#include "string.h"


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
static enum{
	MR0=0,
	CONTRAST=2,
	BRIGHTNESS=3,
	VERY_LOW=10,LOW,MODERATE,HIGH,VERY_HIGH,
	CLKinMHZ=29,        /* 30Mhz - 1*/
	FREQ_10KHZ=100,  	/*Period 100us -> Freq=10Khz */
	FREQ_1KHZ=1000      /*Period 1000us -> Freq=1Khz */
};

/*****************************************************************************
 * Private functions
 ****************************************************************************/
static void Board_LCD_Init(void);
static void Board_Keypad_Init(void);
static void Board_SD_Init(void);
static void Board_CAN_PORT_Init(void);
static void Board_Modem_Init(void);

static void Board_PWM_Init(void);
static void Board_SSP_Init(LPC_SSP_T *pSSP);
static void Board_UART_Init(LPC_USART_T *pUART);
static void Board_CAN_Init(LPC_CAN_T *pCAN);

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Set up and initialize all required blocks and functions related to the
   board hardware */
void Board_Init(void)
{
	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	Board_LCD_Init();
	Board_Keypad_Init();
	Board_SD_Init();
	Board_CAN_PORT_Init();
	Board_Modem_Init();
}


static void Board_LCD_Init(void)
{
	STATIC const PINMUX_GRP_T pinmuxing[] = {

			/*Data port for 64x128 LCD */

		{1,  19,   IOCON_MODE_INACT | IOCON_FUNC0},/*D0*/
		{1,  20,   IOCON_MODE_INACT | IOCON_FUNC0},/*D1*/
		{1,  21,   IOCON_MODE_INACT | IOCON_FUNC0},/*D2*/
		{1,  22,   IOCON_MODE_INACT | IOCON_FUNC0},/*D3*/
		{1,  23,   IOCON_MODE_INACT | IOCON_FUNC0},/*D4*/
		{1,  24,   IOCON_MODE_INACT | IOCON_FUNC0},/*D5*/
		{1,  25,   IOCON_MODE_INACT | IOCON_FUNC0},/*D6*/
		{1,  26,   IOCON_MODE_INACT | IOCON_FUNC0},/*D7*/
			/*Command port for 64x128 LCD */
		{1,  27,   IOCON_MODE_INACT | IOCON_FUNC0}, /*DI*/
		{1,  28,   IOCON_MODE_INACT | IOCON_FUNC0}, /*EN*/
		{1,  29,   IOCON_MODE_INACT | IOCON_FUNC0}, /*RW*/
		{1,  18,   IOCON_MODE_INACT | IOCON_FUNC0}, /*CS1*/
		//{,, },                                    /*CS2*/
	};
	/*LCD port direction is set on Lib_GLCDLowLevel -> MicroInterface.c line 20 */
	Chip_IOCON_SetPinMuxing(LPC_IOCON, pinmuxing, sizeof(pinmuxing) / sizeof(PINMUX_GRP_T));

	/*
	 * The pin mode cannot be selected for pins P0[27] to P0[30]. Pins P0[27] and P0[28]
	 * are dedicated I2C open-drain pins without pull-up/down. Pins P0[29] and P0[30] are
	 * USB specific pins without configurable pull-up or pull-down resistors.
	 * Pins P0[29] and P0[30] also must have the same direction since they operate
	 * as a unit for the USB function
	 * */

	/*LCD contrast and brightness adjustment*/
	Board_PWM_Init();
}


static void Board_PWM_Init(void){

	Chip_PWM_Init(LPC_PWM1);
	Chip_IOCON_PinMuxSet(LPC_IOCON, PORT_PWM_LCD,PIN_PWM_CONTRAST, IOCON_MODE_INACT | IOCON_FUNC3);
	Chip_PWM_PrescaleSet(LPC_PWM1, CLKinMHZ); /*30Mhz to 1 us timebase*/
	Chip_PWM_SetControlMode(LPC_PWM1,CONTRAST, PWM_SINGLE_EDGE_CONTROL_MODE, PWM_OUT_ENABLED);
	Chip_PWM_SetMatch(LPC_PWM1, MR0, FREQ_1KHZ);      /*period 1ms*/
	Chip_PWM_SetMatch(LPC_PWM1, CONTRAST,VERY_LOW ); /*duty 1% */
	LPC_PWM1->LER = (1<<MR0)|(1<<CONTRAST);
	Chip_PWM_Enable(LPC_PWM1);

	return;

}



static void Board_Keypad_Init(void)
{

	/* Keypad port	 */
	STATIC const PINMUX_GRP_T pinmuxing[] = {

	{KEYPAD_PORT_ROWS,  ROW_0,   IOCON_MODE_INACT  | IOCON_FUNC0},
	{KEYPAD_PORT_ROWS,  ROW_1,   IOCON_MODE_INACT  | IOCON_FUNC0},
	{KEYPAD_PORT_ROWS,  ROW_2,   IOCON_MODE_INACT  | IOCON_FUNC0},
	{KEYPAD_PORT_ROWS,  ROW_3,   IOCON_MODE_INACT  | IOCON_FUNC0},
	{KEYPAD_PORT_ROWS,  COLUMN_0,   IOCON_MODE_PULLUP | IOCON_FUNC0},
	{KEYPAD_PORT_ROWS,  COLUMN_1,   IOCON_MODE_PULLUP | IOCON_FUNC0},

	};

	Chip_IOCON_SetPinMuxing(LPC_IOCON, pinmuxing, sizeof(pinmuxing) / sizeof(PINMUX_GRP_T));

	/* OUTPUTS */
	Chip_GPIO_SetPinDIROutput (LPC_GPIO,KEYPAD_PORT_ROWS,ROW_0);
	Chip_GPIO_SetPinDIROutput (LPC_GPIO,KEYPAD_PORT_ROWS,ROW_1);
	Chip_GPIO_SetPinDIROutput (LPC_GPIO,KEYPAD_PORT_ROWS,ROW_2);
	Chip_GPIO_SetPinDIROutput (LPC_GPIO,KEYPAD_PORT_ROWS,ROW_3);

	/* INPUTS */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO,KEYPAD_PORT_COLUMN_0,COLUMN_0);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO,KEYPAD_PORT_COLUMN_1,COLUMN_1);

	/* Configure the GPIO interrupt */
	Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, KEYPAD_PORT_COLUMN_0, 1 << COLUMN_0);
	Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, KEYPAD_PORT_COLUMN_1, 1 << COLUMN_1);

	/* Enable interrupt in the NVIC */
	NVIC_ClearPendingIRQ(GPIO_INTERRUPT_NVIC_NAME);
	NVIC_EnableIRQ(GPIO_INTERRUPT_NVIC_NAME);
}

static void Board_SD_Init(void)
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, SD_DETECT_PORT, SD_DETECT_PIN, IOCON_MODE_INACT);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO,SD_DETECT_PORT,SD_DETECT_PIN);/* Card detect P1.31 Input*/

	Chip_IOCON_PinMuxSet(LPC_IOCON, SD_CS_PORT, SD_CS_PIN, IOCON_MODE_INACT);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,SD_CS_PORT,SD_CS_PIN); /* CS P0.16 Output*/

	Board_SSP_Init(SD_SSP);

	Chip_SSP_Init(SD_SSP);/*Configure Mode, Format and Bit Rate.. Use default configuration*/
	Chip_SSP_SetBitRate(SD_SSP, 1000000);
	Chip_SSP_Enable(SD_SSP);
}

static void Board_Modem_Init(void)
{
	Board_UART_Init(MODEM_UART);
	Chip_UART_Init(MODEM_UART);
	Chip_UART_SetBaud(MODEM_UART,19200);
	Chip_UART_ConfigData(MODEM_UART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS));
	Chip_UART_TXEnable(MODEM_UART);
	/* Enable receive data interrupt */
	Chip_UART_IntEnable(MODEM_UART, UART_IER_RBRINT );
	/* Enable interrupt in the NVIC */

#ifdef MODEM_USE_UART0
	NVIC_EnableIRQ(UART0_IRQn);
#endif

#ifdef MODEM_USE_UART1
	NVIC_EnableIRQ(UART1_IRQn);
#endif

#ifdef MODEM_USE_UART2
	NVIC_EnableIRQ(UART2_IRQn);
#endif

#ifdef MODEM_USE_UART3
	NVIC_EnableIRQ(UART3_IRQn);
#endif

	Chip_IOCON_PinMuxSet(LPC_IOCON, MODEM_RESET_PORT, MODEM_RESET_PIN, IOCON_MODE_INACT);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,MODEM_RESET_PORT,MODEM_RESET_PIN);/* hardware reset P2.12 Input*/
	Chip_GPIO_SetPinState(LPC_GPIO, MODEM_RESET_PORT, MODEM_RESET_PIN, false); /*low initial state*/

}

static void Board_CAN_PORT_Init(void){

	Board_CAN_Init(USE_CAN);
	Chip_CAN_Init(USE_CAN, LPC_CANAF, LPC_CANAF_RAM);
	Chip_CAN_SetBitRate(USE_CAN, CAN_BIT_RATE);
	Chip_CAN_SetAFMode(LPC_CANAF, CAN_AF_BYBASS_MODE);
	Chip_CAN_EnableInt(USE_CAN, CAN_IER_BITMASK);

	NVIC_EnableIRQ(CAN_IRQn);
}

/* Initialize pin muxing for SSP interface */
static void Board_SSP_Init(LPC_SSP_T *pSSP)
{
	if (pSSP == LPC_SSP1) {
		/* Set up clock and muxing for SSP1 interface */
		/*
		 * Initialize SSP0 pins connect
		 * P0.7: SCK
		 * P0.6: SSEL
		 * P0.8: MISO
		 * P0.9: MOSI
		 */
		Chip_IOCON_PinMux(LPC_IOCON, 0, 7, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 6, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 8, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 9, IOCON_MODE_INACT, IOCON_FUNC2);
	}
	else {
		/* Set up clock and muxing for SSP0 interface */
		/*
		 * Initialize SSP0 pins connect
		 * P0.15: SCK
		 * P0.16: SSEL
		 * P0.17: MISO
		 * P0.18: MOSI
		 */
		Chip_IOCON_PinMux(LPC_IOCON, 0, 15, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 16, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 17, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 18, IOCON_MODE_INACT, IOCON_FUNC2);
	}
}


/* Initialize pin muxing for SSP interface */
static void Board_UART_Init(LPC_USART_T *pUART)
{
	if (pUART == LPC_UART0) {
		/* Set up clock and muxing for UART0 interface */
		/*
		 * P0.2: TX - IOCON_FUNC1
		 * P0.3: RX - IOCON_FUNC1
		 */
		Chip_IOCON_PinMux(LPC_IOCON, 0, 2, IOCON_MODE_INACT, IOCON_FUNC1);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 3, IOCON_MODE_INACT, IOCON_FUNC1);
	}
	if (pUART == LPC_UART1) {
			/* Set up clock and muxing for UART1 interface */
			/*
			 * P0.15: TX - IOCON_FUNC1
			 * P0.16: RX - IOCON_FUNC1
			 * P0.17: CTS - IOCON_FUNC1
			 * P0.18: DCD - IOCON_FUNC1
			 * P0.19: DSR - IOCON_FUNC1
			 * P0.20: DTR - IOCON_FUNC1
			 * P0.21: RI - IOCON_FUNC1
			 * P0.22: RTS - IOCON_FUNC1
			 * OR
			 * P2.0: TX - IOCON_FUNC2
			 * P2.1: RX - IOCON_FUNC2
			 * P2.2: CTS - IOCON_FUNC2
			 * P2.3: DCD - IOCON_FUNC2
			 * P2.4: DSR - IOCON_FUNC2
			 * P2.5: DTR - IOCON_FUNC2
			 * P2.6: RI - IOCON_FUNC2
			 * P2.7: RTS - IOCON_FUNC2
			 */
		}
	if (pUART == LPC_UART2) {
		/* Set up clock and muxing for UART2 interface */
		/*
		 * P0.10: TX - IOCON_FUNC1
		 * P0.11: RX - IOCON_FUNC1
		 * OR
		 * P2.8: TX	- IOCON_FUNC2
		 * P2.9: RX - IOCON_FUNC2
		 */
		Chip_IOCON_PinMux(LPC_IOCON, 0, 10, IOCON_MODE_INACT, IOCON_FUNC1);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 11, IOCON_MODE_INACT, IOCON_FUNC1);
	}
	if (pUART == LPC_UART3) {
		/* Set up clock and muxing for UART3 interface */
		/*
		 * P0.0: TX - IOCON_FUNC2
		 * P0.1: RX - IOCON_FUNC2
		 * OR
		 * P0.25: TX - IOCON_FUNC3
		 * P0.26: RX - IOCON_FUNC3
		 * OR
		 * P4.28: TX - IOCON_FUNC3
		 * P4.29: RX - IOCON_FUNC3
		 */
		Chip_IOCON_PinMux(LPC_IOCON, 0, 0, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 1, IOCON_MODE_INACT, IOCON_FUNC2);
	}

}

/* Initialize pin muxing for CAN interface */
static void Board_CAN_Init(LPC_CAN_T *pCAN)
{
	if (pCAN == LPC_CAN1) {
		/* Set up clock and muxing for CAN1 interface */
		/*
		 * Initialize CAN1 pins connect
		 * P0.0: RD1 — CAN1 receiver input (FUNC1).
		 * P0.1: TD1 — CAN1 transmitter output(FUNC1).
		 * or
		 * P0.21: RD1 — CAN1 receiver input (FUNC2).
		 * P0.22: TD1 — CAN1 transmitter output (FUNC2).
		 */
	#if defined(CAN1_PORT0_PIN0_1)
		Chip_IOCON_PinMux(LPC_IOCON, 0, 0, IOCON_MODE_INACT, IOCON_FUNC1);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 1, IOCON_MODE_INACT, IOCON_FUNC1);
	#elif defined(CAN1_PORT0_PIN21_22)
		Chip_IOCON_PinMux(LPC_IOCON, 0, 21, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 22, IOCON_MODE_INACT, IOCON_FUNC2);
	#endif

	}
	else {
		/* Set up clock and muxing for CAN2 interface */
		/*
		 * Initialize CAN2 pins connect
		 * P2.7: RD2 — CAN2 receiver input (FUNC1).
		 * P2.8: TD2 — CAN2 transmitter output (FUNC1).
		 * or
		 * P0.4: RD1 — CAN2 receiver input (FUNC2).
		 * P0.5: TD1 — CAN2 transmitter output(FUNC2).
		 */
	#if defined(CAN2_PORT2_PIN7_8)
		Chip_IOCON_PinMux(LPC_IOCON, 2, 7, IOCON_MODE_INACT, IOCON_FUNC1);
		Chip_IOCON_PinMux(LPC_IOCON, 2, 8, IOCON_MODE_INACT, IOCON_FUNC1);
    #elif defined(CAN2_PORT0_PIN4_5)
		Chip_IOCON_PinMux(LPC_IOCON, 0, 4, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 5, IOCON_MODE_INACT, IOCON_FUNC2);
	#endif

		Chip_IOCON_PinMuxSet(LPC_IOCON, CAN_EN_PORT, CAN_EN_PIN, IOCON_MODE_INACT);
		Chip_GPIO_SetPinDIROutput(LPC_GPIO,CAN_EN_PORT,CAN_EN_PIN);/* hardware reset P2.12 Input*/
		Chip_GPIO_SetPinState(LPC_GPIO, CAN_EN_PORT, CAN_EN_PIN, true); /*low initial state*/

		Chip_IOCON_PinMuxSet(LPC_IOCON, CAN_STB_PORT, CAN_STB_PIN, IOCON_MODE_INACT);
		Chip_GPIO_SetPinDIROutput(LPC_GPIO,CAN_STB_PORT,CAN_STB_PIN);/* hardware reset P2.12 Input*/
		Chip_GPIO_SetPinState(LPC_GPIO, CAN_STB_PORT, CAN_STB_PIN, true); /*low initial state*/

	}
}

