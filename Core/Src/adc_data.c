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
#define ADC_START_BIT        2
#define ADC_CONTROL_REG_ADDR ((uint32_t *)0x40012408)

/* Private variables */
static uint16_t raw_data_buffer[ADC_CHANNELS];

/**
 * @brief Calibrate and Start ADC
 * @param hadc ADC Handler
 * @retval None
 */
void startADC(ADC_HandleTypeDef *hadc)
{
    if (HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_ADC_Start_DMA(hadc, (uint32_t *)raw_data_buffer, ADC_CHANNELS) !=
        HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief Read the 12-bit values from ADC channels
 * @param None
 * @retval Pointer to array of size ADC_CHANNELS with the 12-bit ADC values
 */
const uint16_t *getADCData()
{
    return raw_data_buffer;
}

/**
 * @brief Converts the selected ADC channel to volts
 * @param ADC Channel (1 - ADC_CHANNELS)
 * @retval voltage
 */
float getVolts(uint8_t adc_channel)
{
    float volts =
        ((float)raw_data_buffer[adc_channel - 1]) / (ADC_RES)*ADC_VDDA;
    return volts;
}

/**
 * @brief Trigger ADC reading
 * @param None
 * @retval None
 */
void triggerADC()
{
    /* Change ADCSTART bit in the ADC control register */
    volatile uint32_t *p_adc_control_reg = ADC_CONTROL_REG_ADDR;
    *p_adc_control_reg |= (1 << ADC_START_BIT);
}
