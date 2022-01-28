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

/** ADC_RES should change depending on the resolution selected (4096 for 12-bits) */
#define ADC_RES	     	4096
#define ADC_VDDA     	(3.3)
/** Number of active ADC Channels (Temperature + Telemetry) */
#define ADC_CHANNELS 	14
#define TEMP_CHANNELS 	10
#define TELE_CHANNELS 	(ADC_CHANNELS - TEMP_CHANNELS)

#define TELE			0
#define TEMP 			1


/* Exported Functions Prototypes */
void adcInit(ADC_HandleTypeDef* hadc);
const float* getVolts(uint8_t temp_or_tele);
const float* readTele();
void triggerADC();

#endif /* ADC_DATA_H_ */
