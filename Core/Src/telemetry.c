/**
 * @file telemetry.c
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */


#include "temperature.h"
#include "adc_data.h"
#include "telemetry.h"

/* Private defines */
#define MAX_IN_CURRENT 	20
#define INA180_V		(3.0)

/* Private variables */
float converted_tele_buf[TELE_CHANNELS];


const float* readTele()
{
	const float* tele_volts = getVolts(TELE);

	if(tele_volts == NULL)
	{
		Error_Handler();
	}


	/* Calculate telemetry value and store in buffer */
	float tmp_value;

	for(uint8_t channel = 0; channel < TELE_CHANNELS; channel++)
	{
		/* Store voltage */
		if(channel == 0)
		{
			tmp_value = tele_volts[channel];
			converted_tele_buf[channel] = tmp_value;
		}

		/* Store total and control current */
		if(channel == 1 || channel == 2)
		{
			tmp_value = (tele_volts[channel]/INA180_V)*MAX_IN_CURRENT;
			converted_tele_buf[channel] = tmp_value;
		}

		/* Store Ambient temp */
		if(channel == 3)
		{
			tmp_value = convertTemp(tele_volts[channel]);
			converted_tele_buf[channel] = tmp_value;
		}
	}

	return converted_tele_buf;
}
