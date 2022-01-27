/**
 *
 * @file  adc_data.c
 * @brief This file contains the ADC temperature reading functions
 */

/*  Created on: 10 Jan 2022
 *  Author: niuslar
 */

#include "adc_data.h"
#include "main.h"
#include <math.h>

/* Private defines */
#define ADC_START_BIT 	2
#define MAX_IN_CURRENT 	20
#define INA180_V		((3.0/ADC_VDDA)*ADC_RES)


/* Private variables */
float converted_temp_buf[TEMP_CHANNELS];
float converted_tele_buf[TELE_CHANNELS];
static uint16_t raw_data_buffer[ADC_CHANNELS];

/* Private functions prototypes */
static float convertTemp(uint16_t raw_value);

/**
  * @brief Calibrate and Start ADC
  * @param hadc ADC Handler
  * @retval None
  */
void adcInit(ADC_HandleTypeDef* hadc)
{
	if(HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_ADC_Start_DMA(hadc, (uint32_t*)raw_data_buffer, ADC_CHANNELS) != HAL_OK)
	{
		Error_Handler();
	}

}

/**
  * @brief Read the 12-bit values
  * @param None
  * @retval Pointer to array of size ADC_CHANNELS with the 12-bit ADC values
  */
const uint16_t* readADCData()
{
	return raw_data_buffer;
}

/**
  * @brief Read the temperature from ADC values
  * @param None
  * @retval Pointer to array of size ADC_CHANNELS with the temperature in Celsius
  */
const float* readTempSensors()
{
	/* Loop through each value in the raw_data_buffer and convert it */
	for(int i = 0; i < TEMP_CHANNELS; i++)
	{
		converted_temp_buf[i] = convertTemp(raw_data_buffer[i]);
	}

	return converted_temp_buf;
}

static float convertTemp(uint16_t raw_value)
{
	float temp_celsius;
	/* Check the values are within range (2.67 and 0.57V) */
	if(raw_value > MIN_VRANGE && raw_value < MAX_VRANGE)
	{
		float voltage = ((double)raw_value/ADC_RES)*ADC_VDDA;
		/* Polynomial equation to convert voltage to Celsius
		 * f(x) = 214 + -166x + 69.7x^2 + -13.4x^3, where x is the voltage */
		temp_celsius = 214 - 166*voltage + 69.7*pow(voltage,2) -13.4*pow(voltage,3);
	}
	else
	{
		/* Return unrealistic value to trigger error */
		temp_celsius = 999.9;
	}
	return temp_celsius;
}

const float* readTele()
{
	/* Calculate telemetry value and store in buffer */
	float tmp_value;
	uint8_t channel;

	/* Calculate and store voltage */
	channel = TEMP_CHANNELS;
	tmp_value = ((float)raw_data_buffer[channel]/ADC_RES)*ADC_VDDA;
	converted_tele_buf[0] = tmp_value;

	/* Total current */
	channel = TEMP_CHANNELS + 1;
	tmp_value = ((float)raw_data_buffer[channel]/INA180_V)*MAX_IN_CURRENT;
	converted_tele_buf[1] = tmp_value;

	/* Control Current */
	channel = TEMP_CHANNELS + 2;
	tmp_value = ((float)raw_data_buffer[channel]/INA180_V)*MAX_IN_CURRENT;
	converted_tele_buf[2] = tmp_value;

	/* Ambient temp */
	channel = TEMP_CHANNELS + 3;
	tmp_value = convertTemp(raw_data_buffer[channel]);
	converted_tele_buf[3] = tmp_value;

	return converted_tele_buf;
}

void triggerADC()
{
	/* start ADC again */
	volatile uint32_t* ADC_control_reg = (uint32_t*)0x40012408;

	/* Change ADCSTART bit in the ADC control register */
	*ADC_control_reg |= (1 << ADC_START_BIT);
}



