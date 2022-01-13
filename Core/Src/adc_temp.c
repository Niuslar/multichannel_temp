/*
 * adc_temp.c
 *
 *  Created on: 10 Jan 2022
 *      Author: niuslar
 */

#include "adc_temp.h"
#include "main.h"
#include <math.h>

//Private variables
static uint16_t raw_data_buffer[BUF_DATA_LEN];
static double converted_data_buf[BUF_DATA_LEN];

//Private functions prototypes
static void calibrate_sensor(uint8_t adc_channel);
static double convert_data(uint16_t raw_value);


void temp_init(ADC_HandleTypeDef* hadc)
{
	//Calibrate ADC (SINGLE ENDED ADC is assumed)
	if(HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}

	//Start ADC with DMA
	if(HAL_ADC_Start_DMA(hadc, (uint32_t *)raw_data_buffer, BUF_DATA_LEN) != HAL_OK)
	{
		Error_Handler();
	}

}


double *read_temp_sensors()
{
	//Loop through each value in the raw_data_buffer and convert it
	for(int i = 0; i < BUF_DATA_LEN; i++)
	{
		converted_data_buf[i] = convert_data(raw_data_buffer[i]);
	}

	//Once all the data has been converted and stored in the buffer, start ADC again
	volatile uint32_t* ADC_control_reg = (uint32_t*)0x40012408;
	//Change ADCSTART bit in the ADC control register
	*ADC_control_reg |= (1 << ADC_START_BIT);

	return converted_data_buf;

}


//Choose which ADC channel (1-8) to calibrate
static void calibrate_sensor(uint8_t adc_channel)
{

}

static double convert_data(uint16_t raw_value)
{
	double temp_celsius;
	//Convert raw values (after calibration)
	//Check the values are within range (2.67 and 0.57V)
	if(raw_value > 706 || raw_value < 3316)
	{
		double voltage = ((double)raw_value/ADC_RES)*ADC_VDDA;
		//Polynomial equation to convert voltage to Celsius
		//f(x) = 214 + -166x + 69.7x^2 + -13.4x^3, where x is the voltage
		temp_celsius = 214 - 166*voltage + 69.7*pow(voltage,2) -13.4*pow(voltage,3);
	}
	else
	{
		//Return unrealistic value to trigger error
		temp_celsius = 999.9;
	}
	return temp_celsius;
}


