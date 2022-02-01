/**
 * @file temperature.c
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */

#include "temperature.h"
#include "adc_data.h"
#include "math.h"

/* Private defines */
#define MIN_VRANGE (0.569)
#define MAX_VRANGE (2.672)

/**
 * @brief Convert ADC voltage to temperature in Celsius for selected channel
 * @param adc channel from 0 to ADC_CHANNELS - 1
 * @retval temperature in Celsius
 * Celsius
 */
const float readTemperature(uint8_t adc_channel)
{
    if (adc_channel < 0 || adc_channel >= ADC_CHANNELS)
    {
        Error_Handler();
    }
    float volts;
    float temperature;

    volts = getVolts(adc_channel);
    temperature = convertTemperature(volts);

    return temperature;
}

float convertTemperature(float volts)
{
    float temp_celsius;
    /* Check the values are within range (2.67 and 0.57V) */
    if (volts > MIN_VRANGE && volts < MAX_VRANGE)
    {
        /* Polynomial equation to convert voltage to Celsius
         * f(x) = 214 + -166x + 69.7x^2 + -13.4x^3, where x is the voltage */
        temp_celsius =
            214 - 166 * volts + 69.7 * pow(volts, 2) - 13.4 * pow(volts, 3);
    }
    else
    {
        /* Return unrealistic value to trigger error */
        temp_celsius = OUT_OF_RANGE_ERROR;
    }
    return temp_celsius;
}
