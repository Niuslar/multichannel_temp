/**
 * @file telemetry.c
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */

#include "telemetry.h"
#include "adc_data.h"
#include "temperature.h"

/* Private defines */
#define MAX_IN_CURRENT_AMP 20
#define INA180_VOLT        (3.0)

/**
 * @brief Internal telemetry readings for selected channel
 * @param adc channel
 * @retval Internal telemetry reading
 */
const float readTelemetry(uint8_t adc_channel)
{
    float volts;
    float telemetry;

    /* Check that the ADC channel is a telemetry channel */
    if (adc_channel <= TEMP_CHANNELS || adc_channel > ADC_CHANNELS)
    {
        Error_Handler();
    }

    /* Voltage channel */
    if (adc_channel == 11)
    {
        telemetry = getVolts(adc_channel);
    }

    /* Total and control current channels */
    if (adc_channel == 12 || adc_channel == 13)
    {
        volts = getVolts(adc_channel);
        telemetry = (volts / INA180_VOLT) * MAX_IN_CURRENT_AMP;
    }

    /* Ambient temperature */
    if (adc_channel == 14)
    {
        volts = getVolts(adc_channel);
        telemetry = convertTemperature(volts);
    }

    return telemetry;
}
