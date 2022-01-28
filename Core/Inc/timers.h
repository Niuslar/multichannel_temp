/**
 * @file timers.h
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "main.h"

#define CONTROL_CHANNELS	10

typedef struct{
	TIM_HandleTypeDef* htim;
	uint8_t	timer_ch;
	uint8_t control_ch;
}pwm_handler_t;

/* Exported Function Prototypes */
void setDutyCycle(pwm_handler_t* p_pwm_handler, uint8_t duty_cycle);

#endif /* TIMERS_H_ */
