/**
 * @file adc_data.h
 * @brief macros and function prototypes for ADC temperature reading
 */

/* Created on: 10 Jan 2022
 * Author: niuslar
 */

#ifndef INC_ADC_DATA_H_
#define INC_ADC_DATA_H_

#include "main.h"

/** ADC_RES should change depending on the resolution selected (4096 for 12-bits) */
#define ADC_RES	     	4096
#define ADC_VDDA     	(3.3)
#define MIN_VRANGE   	(0.569/ADC_VDDA*(ADC_RES))
#define MAX_VRANGE	 	(2.672/ADC_VDDA*(ADC_RES))
/** Number of active ADC Channels (Temperature + Telemetry) */
#define ADC_CHANNELS 	14
#define TEMP_CHANNELS 	10
#define TELE_CHANNELS 	(ADC_CHANNELS - TEMP_CHANNELS)


/* Exported Functions Prototypes */
void adcInit(ADC_HandleTypeDef* hadc);
const float* readTempSensors();
const float* readTele();
const uint16_t* readADCData();
void triggerADC();

#endif /* INC_ADC_DATA_H_ */
