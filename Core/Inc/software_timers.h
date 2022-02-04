/**
 * @file software_timers.h
 */

/*
 *  Created on: 4 Feb 2022
 *      Author: niuslar
 */

#ifndef SOFTWARE_TIMERS_H_
#define SOFTWARE_TIMERS_H_

#include "main.h"

#define DUTY_STEPS 100

typedef struct
{
    TIM_HandleTypeDef *p_htim;
    uint16_t control_pin;
} soft_pwm_handler_t;

void startSoftPWM(soft_pwm_handler_t *p_soft_pwm_h);
void setSoftDutyCycle(soft_pwm_handler_t *p_soft_pwm_h,
                      uint8_t duty_cycle_percent);
uint16_t *getDutyCycle();

#endif /* SOFTWARE_TIMERS_H_ */
