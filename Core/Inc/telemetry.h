/**
 * @file telemetry.h
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include "stdint.h"

/* Exported Function Prototypes */
const float readTelemetry(uint8_t adc_channel);

#endif /* TELEMETRY_H_ */
