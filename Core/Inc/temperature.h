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

/* Exported Function Prototypes */
const float readTemperature(uint8_t adc_channel);
float convertTemperature(float volts);

#endif /* TEMPERATURE_H_ */
