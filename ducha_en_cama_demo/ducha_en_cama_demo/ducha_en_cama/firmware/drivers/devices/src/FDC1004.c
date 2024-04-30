/*
 * MIT License
 *
 */
/*==================[inclusions]=============================================*/
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "FDC1004.h"

/*==================[internal data declaration]==============================*/
uint8_t MEAS_CONFIG[] = {0x08, 0x09, 0x0A, 0x0B}; // Configuración de la medición 1, 2, 3 y 4
uint8_t MEAS_MSB[] = {0x00, 0x02, 0x04, 0x06}; // bit más significativo de cada medición 1, 2, 3 y 4 
uint8_t MEAS_LSB[] = {0x01, 0x03, 0x05, 0x07};
uint8_t FDC1004_capdac_values[]  = { 0, 0, 0, 0 };
uint8_t SAMPLE_DELAY[] = {11, 11, 6, 3};
double _base_cap;


static const char *TAG = "fdc1004";

void FDC1004_Init(uint16_t rate, int sda_io, int scl_io){

uint32_t i2c_clk;
	if (rate == FDC1004_100HZ)
	{
		i2c_clk = 100000u;} 
	else if (rate == FDC1004_200HZ)
	{
		i2c_clk = 200000u; }
	else
	{
		i2c_clk = 400000u; }

	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = sda_io;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = scl_io;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = i2c_clk;
	conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;
	esp_err_t err = i2c_param_config(I2C_NUM_0, &conf);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "fdc1004 begin error %s", esp_err_to_name(err));
	}
	err = i2c_driver_install(I2C_NUM_0, conf.mode, 0u, 0u, 0u);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "i2c_driver_install error %s", esp_err_to_name(err));
	}
}

void FDC1004_write16(uint8_t reg, uint16_t data){

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ADDR << 1 | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, reg, true);
	i2c_master_write_byte(cmd, (uint8_t)(data >> 8), true);
	i2c_master_write_byte(cmd, (uint8_t)(data & 0xFF), true);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

}

uint16_t FDC1004_read16(uint8_t reg){
	uint8_t msb_data;
	uint8_t lsb_data;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ADDR << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, reg, true);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ADDR << 1) | I2C_MASTER_READ, true);
	i2c_master_read_byte(cmd, &msb_data, I2C_MASTER_ACK);
	i2c_master_read_byte(cmd, &lsb_data, I2C_MASTER_NACK);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
	return ((msb_data << 8u) | lsb_data);
}

/*
 * @brief Configure a single measurement.
 * @details Write a measurement configuration to the FDC. A measurement
 * configuration consists out of the channel to use, the measurement mode and
 * the value for the CAPDAC.
 *
 * @param measurement Measurement ID (0 - 3)
 * @param channel Channel ID (0 - 3)
 * @param capdac Capacity offset (C_offset = capdac * 3.125 pF)
 * @return 0 if 
 */
uint8_t FDC1004_configureMeasurementSingle(uint8_t measurement, uint8_t channel, uint8_t capdac){
// Verify data
	if (!FDC1004_IS_MEAS(measurement) || !FDC1004_IS_CHANNEL(channel) || capdac > FDC1004_CAPDAC_MAX)
	{
		ESP_LOGE(TAG, "bad configuration");
		return 1;
	}

	// build 16 bit configuration
	uint16_t configuration_data;
	configuration_data = ((uint16_t)channel) << 13; // CHA
	configuration_data |= ((uint16_t)0x04) << 10;	// CHB disable / CAPDAC enable
	configuration_data |= ((uint16_t)capdac) << 5;	// CAPDAC value
	FDC1004_capdac_values[measurement] = capdac;
	FDC1004_write16(MEAS_CONFIG[measurement], configuration_data);
  
	return 0;
}


/*
 * @brief Configure a differential measurement.
 * @details Write a measurement configuration to the FDC. A measurement
 * configuration consists on the channel to use, the measurement mode and the
 * value for the CAPDAC.
 *
 * @param measurement Measurement ID (0 - 3)
 * @param channel Channel ID (0 - 3)
 * @param capdac Capacity offset (C_offset = capdac * 3.125 pF)
 * @return 0 
 */
uint8_t FDC1004_configure_differential_measurement(uint8_t measurement, uint8_t channel_a, uint8_t channel_b) {
   /* Measurement configuration register:
         [15:13] = Positive channel number
         [12:10] = Negative channel number (or CAPDAC/DISABLE)
         [ 9:5 ] = CAPDAC 0b00000 - 0b11111 (C_offset = CAPDAC * 3.125 pF)
         [ 4:0 ] = RESERVED, always 0
   */
   uint16_t conf_data = ((uint16_t)channel_a) << 13;
   conf_data |= ((uint16_t)channel_b) << 10;
   conf_data |= ((uint16_t)0b00000) << 5;
   FDC1004_write16(MEAS_CONFIG[measurement], conf_data);
   return 0;
}

/*
 * @brief Trigger a measurement.
 * @details Trigger a measurement. Note that the measurement is only triggered
 * here, it is not read out nor waits this function on the completion of the
 * measurement. For reading the measurement directly please use
 * FDC1004_measure_channel.
 *
 * @param measurement Measurement ID (0 - 3)
 * @return Success code
 */
uint8_t FDC1004_triggerSingleMeasurement(uint8_t measurement, uint8_t rate){

	// verify data
	if (!FDC1004_IS_MEAS(measurement) || !FDC1004_IS_RATE(rate))
	{
		ESP_LOGE(TAG, "bad trigger request");
		return 1;
	}
	uint16_t trigger_data=0;
	trigger_data = ((uint16_t)rate) << 10;	  // sample rate
	trigger_data |= 0 << 8;					  // repeat disabled
	trigger_data |= (1 << (7 - measurement)); // 0 > bit 7, 1 > bit 6, etc
	FDC1004_write16(FDC_REGISTER, trigger_data);
	return 0;
}


uint8_t FDC1004_readMeasurement(uint8_t measurement, uint16_t *value) {
	if (!FDC1004_IS_MEAS(measurement)) {
		   printf("bad read request");
		return 1;
	}

	//check if measurement is complete
	uint16_t fdc_register = FDC1004_read16(FDC_REGISTER);
	if (!(fdc_register & (1 << (3 - measurement)))) {
		printf("measurement not completed");
		return 2;
	}

	//read the value
	uint16_t msb = FDC1004_read16(MEAS_MSB[measurement]);
	uint16_t lsb = FDC1004_read16(MEAS_LSB[measurement]);
	value[0] = msb;
	value[1] = lsb;
	return 0;
}


uint8_t FDC1004_getRawCapacitance(uint8_t measurement, int32_t *value) {
	//if (!FDC1004_IS_CHANNEL(channel_1) || !FDC1004_IS_CHANNEL(channel_1) || (channel_1 == channel_2)) return 1;

	uint16_t raw_value[2];
	FDC1004_readMeasurement(measurement, raw_value);

	/*if (measureChannel(measurement, channel_1, channel_2, raw_value)) {
		printf("error");
		return 1;
	}*/

	//*value = ((uint32_t)raw_value[1] >> 8) | ((uint32_t)
	
	uint32_t msb = raw_value[0];
	uint32_t lsb = raw_value[1];
	 /* Data format:
    *    MSB[15:0] - 16 MSB of measurement
    *    LSB[15:8] - 8 LSB of measurement
    *    LSB[ 7:0] - Reserved, always 0
    */
   (*value) = (msb << 8) | (lsb >> 8);
	//Convert Two's complement to signed integer (necessary since the data is shifted by 8)
   uint32_t raw_result = (msb << 16) | lsb;
   if (raw_result & ((uint32_t)1 << 31)) {
      (*value) |= ((uint32_t)0xFF << 24);
   }

	return 0;
}

double FDC1004_getCapacitance(uint8_t measurement) {
	int32_t value;
	uint8_t result = FDC1004_getRawCapacitance(measurement, &value);
	if (result) return 0x80000000;

	return (double)value / PICOFARAD_CONVERSION_CONSTANT; //picofarads
}

int16_t float_to_fixed_signed(float input, uint8_t fract_bits)
{
    return (int16_t)(input * (1 << fract_bits));
}
 
void FDC_SetOffsetCalibration(uint8_t channel, float offset)
{
    uint16_t offset_raw = float_to_fixed_signed(offset, FIXED_POINT_FRACTIONAL_BITS_OFFSET);
    uint8_t temp[2] = {offset_raw >> 8, offset_raw & 0xFF};
    FDC1004_write16(FDC1004_OFFSET_CAL_CIN1, 0xC800);

}

void FDC1004_Reset(){

	uint16_t reset = 1 << 15;
	FDC1004_write16(FDC_REGISTER, reset);
}

