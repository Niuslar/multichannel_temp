/*
 * adc_temp.h
 *
 *  Created on: 10 Jan 2022
 *      Author: niuslar
 */

#ifndef INC_ADC_TEMP_H_
#define INC_ADC_TEMP_H_

#include "main.h"

#define BUF_DATA_LEN 8
#define ADC_START_BIT 2
//ADC_RES should change depending on the resolution selected (4096 for 12-bits)
#define ADC_RES	      4096
#define ADC_VDDA    (3.3)
#define MIN_VRANGE  706
#define MAX_VRANGE	3316


//APIs
void temp_init(ADC_HandleTypeDef* hadc);
void read_temp_sensors();


#endif /* INC_ADC_TEMP_H_ */
