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

#define CONTROL_CHANNELS 8

/**
 * @note The value of timer_ch must we one of the following:
 * 			TIM_CHANNEL_1
 * 			TIM_CHANNEL_2
 * 			TIM_CHANNEL_3
 * 			TIM_CHANNEL_4
 */
typedef struct
{
    TIM_HandleTypeDef *p_htim;
    uint8_t timer_ch;
} pwm_handler_t;

enum CONTROL_CH
{
    CONTROL_CH_1,
    CONTROL_CH_2,
    CONTROL_CH_3,
    CONTROL_CH_4,
    CONTROL_CH_5,
    CONTROL_CH_6,
    CONTROL_CH_7,
    CONTROL_CH_8,
};

/* Exported Function Prototypes */
void startPWM(pwm_handler_t *p_pwm_handler);
void setDutyCycle(pwm_handler_t *p_pwm_handler, uint8_t duty_cycle_percent);

#endif /* TIMERS_H_ */
