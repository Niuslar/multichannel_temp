/**
 * @file adc_data.h
 * @brief ADC Data gathering public macros and functions
 */

/* Created on: 10 Jan 2022
 * Author: niuslar
 */

#ifndef ADC_DATA_H_
#define ADC_DATA_H_

#include "main.h"

/** ADC_RES should change depending on the resolution selected (4096 for
 * 12-bits) */
#define ADC_RES  4096
#define ADC_VDDA (3.3)
/** Number of active ADC Channels (Temperature + Telemetry)
 * 	The Temperature ADC Channels should come first and be consecutive
 */
#define ADC_CHANNELS  14
#define TEMP_CHANNELS 10
#define CHANNEL_OK    0
#define CHANNEL_ERROR 1

/* Exported Functions Prototypes */
void startADC(ADC_HandleTypeDef *hadc);
float getVolts(uint8_t adc_channel);
const uint16_t *getADCData();
uint8_t checkChannel(uint8_t adc_channel);
void triggerADC();

#endif /* ADC_DATA_H_ */
