#ifndef GPIO_MCU_H
#define GPIO_MCU_H
/** \addtogroup Drivers_Programable Drivers Programable
 ** @{ */
/** \addtogroup Drivers_Microcontroller Drivers microcontroller
 ** @{ */
/** \addtogroup GIOP GPIO
 */

/*==================[inclusions]=============================================*/
#include <stdbool.h>
#include <stdint.h>
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** @typedef io_t
 * @brief  
 */
typedef enum {GPIO_INPUT = 0, GPIO_OUTPUT} io_t;

/** @typedef gpio_t
 *  @brief  Define los puertos de la EDU-CIAA
 */
typedef enum gpio_list{
	GPIO_0=0, /**<  */
	GPIO_1, /**<  */
	GPIO_2, /**<  */
	GPIO_3, /**<  */
	GPIO_4, /**<  */
	GPIO_5, /**<  */
	GPIO_6, /**<  */
	GPIO_7, /**<  */
	GPIO_8, /**<  */
	GPIO_9, /**<  */
	GPIO_10, /**<  */
	GPIO_11, /**<  */
	GPIO_12, /**<  */
	GPIO_13, /**<  */
	GPIO_14, /**<  */
	GPIO_15, /**<  */
	GPIO_16, /**<  */
	GPIO_17, /**<  */
	GPIO_18, /**<  */
	GPIO_19, /**<  */
	GPIO_20, /**<  */
	GPIO_21, /**<  */
	GPIO_22, /**<  */
	GPIO_23, /**<  */
} gpio_t;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
/** @fn void GPIOInit(void)
 * @brief Inicializacion de los puertos GPIO de la EDU-CIAA
 */
void GPIOInit(gpio_t pin, io_t io);

/** @fn void GPIOOn(gpio_t pin)
 * @brief Pone a 1 un pin de la EDU-CIAA
 * @param[in] pin Pin que se desea poner a 1
 */
void GPIOOn(gpio_t pin);

/** @fn void GPIOOff(gpio_t pin)
 * @brief Apaga un pin de la EDU-CIAA
 * @param[in] pin Pin que se desea poner a 0
 */
void GPIOOff(gpio_t pin);

/** @fn void GPIOState(gpio_t pin)
 * @brief Indica el estado de un pin de la EDU-CIAA
 * @param[in] pin Pin que se desea prender o apagar
 * @param[in] state Estado del pin (1: prendido o 0: apagado)
 */
void GPIOState(gpio_t pin, bool state);

/** @fn void GPIOStateFast(gpio_t pin)
 * @brief Indica el estado de un pin de la EDU-CIAA
 * @param[in] pin Pin que se desea prender o apagar
 * @param[in] state Estado del pin (1: prendido o 0: apagado)
 */
void GPIOStateFast(gpio_t pin, bool state);

/** @fn void GPIOToggle(gpio_t pin)
 * @brief Invierte el estado de un pin de la EDU-CIAA
 * @param[in] pin Pin que se desea invertir
 */
void GPIOToggle(gpio_t pin);

/** @fn bool GPIORead(gpio_t pin)
 * @brief Lectura de un pin de la EDU-CIAA
 * @param[in] pin Pin a leer el estado
 * @return Booleano que indica el estado del pin
 */
bool GPIORead(gpio_t pin);

/** @brief Activa la interrupcion de un pin en particular 
 * @param[in] pin Pin que va a interrumpir
 * @param[in] void *ptr_int_func Funcion a la que se va a llamar en la interrupcion
 * @param[in] bool edge 1 si es flanco ascendente y 0 si es descendente
 * @param[in] args parámetros para la función de interrupción
 */
void GPIOActivInt(gpio_t pin, void *ptr_int_func, bool edge, void *args);

/** @brief Deinicializacion de los puertos GPIO de la EDU-CIAA
 */
void GPIODeinit(void);

/*==================[end of file]============================================*/
#endif /* INC_GPIO_H_ */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
