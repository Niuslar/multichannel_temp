/**
 * @file temperature.h
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "stdint.h"

#define OUT_OF_RANGE_ERROR 999.9

/* Exported Function Prototypes */
const float readTemperature(uint8_t adc_channel);
float convertTemperature(float volts);

#endif /* TEMPERATURE_H_ */
