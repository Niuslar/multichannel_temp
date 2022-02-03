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
#define DUTY_STEPS       100

typedef struct
{
    TIM_HandleTypeDef *p_htim;
    uint8_t timer_ch;
    uint8_t control_ch;
    uint16_t control_pin; /* This is only necessary for software timers */
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
    CONTROL_CH_9,
    CONTROL_CH_10,
};

/* Exported Function Prototypes */
void startPWM(pwm_handler_t *p_pwm_handler);
void setDutyCycle(pwm_handler_t *p_pwm_handler, uint8_t duty_cycle);
void setFrequency(pwm_handler_t *p_pwm_handler, uint32_t freq_hz);
uint32_t *getDutyCycle();

#endif /* TIMERS_H_ */
