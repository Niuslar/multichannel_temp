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

#define CONTROL_CHANNELS 10

typedef struct
{
    TIM_HandleTypeDef *p_htim;
    uint8_t timer_ch;
    uint8_t control_ch;
    uint32_t counter_period;
} pwm_handler_t;

/* Exported Function Prototypes */
void startPWM(uint8_t control_channel,
              uint8_t timer_channel,
              TIM_HandleTypeDef *p_htim);
void setDutyCycle(uint8_t control_channel, uint8_t duty_cycle);
void setFrequency(TIM_HandleTypeDef *p_htim, uint32_t freq_kz);

#endif /* TIMERS_H_ */
