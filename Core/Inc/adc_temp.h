/**
 * @file adc_temp.h
 * @brief macros and function prototypes for ADC temperature reading
 */

/* Created on: 10 Jan 2022
 * Author: niuslar
 */

#ifndef INC_ADC_TEMP_H_
#define INC_ADC_TEMP_H_

#include "main.h"

/**
 * @def ADC_RES
 * @brief ADC_RES should change depending on the resolution selected (4096 for
 * 12-bits)
 */

#define ADC_START_BIT 2
#define ADC_RES       4096
#define ADC_VDDA      (3.3)
#define MIN_VRANGE    (0.569 / ADC_VDDA * (ADC_RES))
#define MAX_VRANGE    (2.672 / ADC_VDDA * (ADC_RES))

/* Exported Functions Prototypes */
void tempInit(ADC_HandleTypeDef *hadc);
const float *readTempSensors();
const uint16_t *readADCData();

#endif /* INC_ADC_TEMP_H_ */
