/**
 *
 * @file  adc_data.c
 * @brief This file contains the ADC data gathering functions
 */

/*  Created on: 10 Jan 2022
 *  Author: niuslar
 */

#include "adc_data.h"

/* Private defines */
#define ADC_START_BIT 	2


/* Private variables */
static uint16_t raw_data_buffer[ADC_CHANNELS];
float temp_volt[TEMP_CHANNELS];
float tele_volt[TELE_CHANNELS];

/* Private functions */
static float adcToVolt(uint16_t raw_adc_value);


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
  * @brief Read the 12-bit values from ADC channels
  * @param None
  * @retval Pointer to array of size ADC_CHANNELS with the 12-bit ADC values
  */
const uint16_t* getADCData()
{
	return raw_data_buffer;
}

/**
  * @brief Converts the selected ADC channels to volts
  * @param temp_or_tele can be either TELE or TEMP
  * @retval Pointer to array of size TEMP_CHANNELS containing voltages
  */
const float* getVolts(uint8_t temp_or_tele)
{
	if(temp_or_tele == TEMP)
	{
		for(uint8_t channel = 0; channel < TEMP_CHANNELS; channel++)
		{
			temp_volt[channel] = adcToVolt(raw_data_buffer[channel]);
		}

		return temp_volt;
	}

	else if(temp_or_tele == TELE)
	{
		for(uint8_t channel = 0; channel < TELE_CHANNELS; channel++)
		{
			tele_volt[channel] = adcToVolt(raw_data_buffer[TEMP_CHANNELS + channel]);
		}

		return tele_volt;
	}

	else
	{
		return NULL;
	}
}


/**
  * @brief Trigger ADC reading
  * @param None
  * @retval None
  */
void triggerADC()
{
	volatile uint32_t* ADC_control_reg = (uint32_t*)0x40012408;

	/* Change ADCSTART bit in the ADC control register */
	*ADC_control_reg |= (1 << ADC_START_BIT);
}

static float adcToVolt(uint16_t raw_adc_value)
{
	float volts = (((float)raw_adc_value)/(ADC_RES))*ADC_VDDA;
	return volts;
}



