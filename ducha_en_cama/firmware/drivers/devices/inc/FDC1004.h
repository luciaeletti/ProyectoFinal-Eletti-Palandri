#ifndef FDC1004_H
#define FDC1004_H
/*
 * 
 */


/*==================[inclusions]=============================================*/
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c_mcu.h"

/*==================[macros]=================================================*/
// Constants and limits for FDC1004
#define ADDR 0b1010000


#define FDC1004_100HZ (0x01)
#define FDC1004_200HZ (0x02)
#define FDC1004_400HZ (0x03)
#define FDC1004_IS_RATE(x) (x == FDC1004_100HZ || \
                            x == FDC1004_200HZ || \
                            x == FDC1004_400HZ)

#define FDC1004_CAPDAC_MAX (0x1F)

#define FDC1004_CHANNEL_MAX (0x03)
#define FDC1004_IS_CHANNEL(x) (x >= 0 && x <= FDC1004_CHANNEL_MAX)
#define FDC1004_CIN1 0
#define FDC1004_CIN2 1
#define FDC1004_CIN3 2
#define FDC1004_CIN4 3

#define FDC1004_MEAS_MAX (0x03)
#define FDC1004_IS_MEAS(x) (x >= 0 && x <= FDC1004_MEAS_MAX)
#define FDC1004_MEAS1 0
#define FDC1004_MEAS2 1
#define FDC1004_MEAS3 2
#define FDC1004_MEAS4 3

#define FDC1004_OFFSET_CAL_CIN1 0x0D
#define FDC1004_OFFSET_CAL_CIN2 0x0E
#define FDC1004_OFFSET_CAL_CIN3 0x0F
#define FDC1004_OFFSET_CAL_CIN4 0x10


#define PICOFARAD_CONVERSION_CONSTANT (524288) // 2^19

#define FDC_REGISTER (0x0C)

#define ATTOFARADS_UPPER_WORD (457) // number of attofarads for each 8th most lsb (lsb of the upper 16 bit half-word)
#define FEMTOFARADS_CAPDAC (3028)   // number of femtofarads for each lsb of the capdac

#define FDC1004_LOWER_LIMIT        -8388500
#define FDC1004_UPPER_LIMIT         8388500
/**
*   \brief Number of fractional bits in offset register.
*/
#define FIXED_POINT_FRACTIONAL_BITS_OFFSET 11

/*==================[typedef]================================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

void FDC1004_Init(uint16_t rate, int sda_io, int scl_io);
void FDC1004_write16(uint8_t reg, uint16_t data);
uint16_t FDC1004_read16(uint8_t reg);
uint8_t FDC1004_configureMeasurementSingle(uint8_t measurement, uint8_t channel, uint8_t capdac);
uint8_t FDC1004_configure_differential_measurement(uint8_t measurement, uint8_t channel_a, uint8_t channel_b) ;
uint8_t FDC1004_triggerSingleMeasurement(uint8_t measurement, uint8_t rate);
uint8_t FDC1004_readMeasurement(uint8_t measurement, uint16_t *value);
uint8_t FDC1004_measureChannel(uint8_t measurement, uint16_t *value);
double FDC1004_getCapacitance(uint8_t measurement);
uint8_t FDC1004_getRawCapacitance(uint8_t measurement, int32_t * value);
double FDC1004_measureLevel(double prefactor);
void FDC1004_setBaseCapacitance();
void FDC_SetOffsetCalibration(uint8_t channel, float offset);
void FDC1004_Reset();


//uint8_t FDC1004_read_raw_measurement(uint8_t measurement, int32_t *result);
//uint8_t FDC1004_read_measurement(uint8_t measurement, double *result);
//uint8_t FDC1004_measureChannel(uint8_t channel, uint16_t rate, uint8_t capdac, uint16_t *value);


//uint8_t FDC1004_readMeasurement(uint8_t measurement, uint16_t *value);
//int32_t FDC1004_getCapacitance(uint8_t channel);
//uint8_t FDC1004_getRawCapacitance(uint8_t channel, fdc1004_measurement_t *value);

#endif   #ifndef FDC1004_H

