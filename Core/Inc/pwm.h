/**
 * @file pwm.h
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */

#ifndef PWM_H_
#define PWM_H_

#include "main.h"
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

/* Exported Function Prototypes */
pwm_handler_t startPWM(TIM_HandleTypeDef *p_htim, uint8_t TIM_CHANNEL_X);
void setDutyCycle(pwm_handler_t *p_pwm_handler, uint8_t duty_cycle_percent);

#endif /* PWM_H_ */
