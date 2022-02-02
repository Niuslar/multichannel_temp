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
    /* Check that the ADC channel is a telemetry channel */
    if (adc_channel < TEMP_CHANNELS || adc_channel >= ADC_CHANNELS)
    {
        Error_Handler();
    }

    float volts;
    float telemetry;

    /* Voltage channel */
    if (adc_channel == 10)
    {
        float adc_voltage = getVolts(adc_channel);
        /* To calculate input voltage before the voltage divide we use
         * input_voltage = adc_voltage*(R1+R2)/R2;
         * where R1 is 28kOhm and R2 is 3.01 kOhm
         */
        telemetry = adc_voltage * (31.01) / (3.01);
    }

    /* Total and control current channels */
    if (adc_channel == 11 || adc_channel == 12)
    {
        volts = getVolts(adc_channel);
        telemetry = (volts / INA180_VOLT) * MAX_IN_CURRENT_AMP;
    }

    /* Ambient temperature */
    if (adc_channel == 13)
    {
        volts = getVolts(adc_channel);
        telemetry = convertTemperature(volts);
    }

    return telemetry;
}
