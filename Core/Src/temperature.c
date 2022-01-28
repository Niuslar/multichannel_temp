/**
 * @file temperature.c
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */

#include "adc_data.h"
#include "temperature.h"
#include "math.h"

/* Private defines */
#define MIN_VRANGE   	(0.569)
#define MAX_VRANGE	 	(2.672)

/* Private variables */
float converted_temp_buf[TEMP_CHANNELS];


/**
  * @brief Convert ADC voltage to temperature in Celsius
  * @param None
  * @retval Pointer to array of size TEMP_CHANNELS with the temperature in Celsius
  */
const float* readTemp()
{
	const float* volts = getVolts(TEMP);

	if(volts == NULL)
	{
		Error_Handler();
	}

	/* Loop through each value in the raw_data_buffer and convert it */
	for(int i = 0; i < TEMP_CHANNELS; i++)
	{
		converted_temp_buf[i] = convertTemp(volts[i]);
	}

	return converted_temp_buf;
}

float convertTemp(float temp_volts)
{
	float temp_celsius;
	/* Check the values are within range (2.67 and 0.57V) */
	if(temp_volts > MIN_VRANGE && temp_volts < MAX_VRANGE)
	{
		/* Polynomial equation to convert voltage to Celsius
		 * f(x) = 214 + -166x + 69.7x^2 + -13.4x^3, where x is the voltage */
		temp_celsius = 214 - 166*temp_volts + 69.7*pow(temp_volts,2) -13.4*pow(temp_volts,3);
	}
	else
	{
		/* Return unrealistic value to trigger error */
		temp_celsius = 999.9;
	}
	return temp_celsius;
}
