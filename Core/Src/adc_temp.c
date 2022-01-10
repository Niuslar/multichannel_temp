/*
 * adc_temp.c
 *
 *  Created on: 10 Jan 2022
 *      Author: niuslar
 */

#include "adc_temp.h"
#include "main.h"

//Private variables
static uint16_t raw_data_buffer[BUF_DATA_LEN];
static uint8_t converted_data_buf[BUF_DATA_LEN]
static uint8_t convert_data(uint16_t raw_value);

//Private functions prototypes
static void calibrate_sensor(uint8_t adc_channel);
static void convert_data(uint16_t data_buffer);


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


void read_temp_sensors()
{
	//Loop through each value in the raw_data_buffer and convert it
	for(int i = 0; i < BUF_DATA_LEN; i++)
	{
		convert_data(raw_data_buffer[i]);
	}

}


//Choose which ADC channel (1-8) to calibrate
static void calibrate_sensor(uint8_t adc_channel)
{

}

static void convert_data(uint16_t raw_value)
{
	//Convert raw values (after calibration)
}


