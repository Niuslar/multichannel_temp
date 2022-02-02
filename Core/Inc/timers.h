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

typedef enum
{
    CONTROL_CH_1,
    CONTROL_CH_2,
    CONTROL_CH_3,
    CONTROL_CH_4,
    CONTROL_CH_5,
    CONTROL_CH_6,
    CONTROL_CH_7,
    CONTROL_CH_8,
    CONTROL_CH_9,
    CONTROL_CH_10,
} control_ch_t;

/* Exported Function Prototypes */
void startPWM(control_ch_t control_channel,
              uint8_t timer_channel,
              TIM_HandleTypeDef *p_htim);
void setDutyCycle(control_ch_t control_channel, uint8_t duty_cycle);
void setFrequency(TIM_HandleTypeDef *p_htim, uint32_t freq_kz);

#endif /* TIMERS_H_ */
