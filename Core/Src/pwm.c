/**
 * @file pwm.c
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */

#include "pwm.h"

/** All timers start with a counter period of 1600
 *  and a prescaler of 400. With a 32MHz clock source, this is 50Hz.
 *  To change this frequency use the Device Configuration Tool.
 */
#define CLOCK_SOURCE_HZ 32000000
#define MIN_DUTY_CYCLE  0
#define MAX_DUTY_CYCLE  100

/**
 * @brief Start PWM
 * @param p_htim -> pointer to the timer handler
 * @param TIM_CHANNEL_X is the timer channel and it can be one of the following:
 *   		TIM_CHANNEL_1
 * 			TIM_CHANNEL_2
 * 			TIM_CHANNEL_3
 * 			TIM_CHANNEL_4
 * @retval pwm_handler
 * */
pwm_handler_t startPWM(TIM_HandleTypeDef *p_htim, uint8_t TIM_CHANNEL_X)
{
    pwm_handler_t pwm_handler;

    /* Check p_htim is valid*/
    if (p_htim == NULL)
    {
        Error_Handler();
    }

    /* Check timer channels are valid */
    if (TIM_CHANNEL_X != TIM_CHANNEL_1 && TIM_CHANNEL_X != TIM_CHANNEL_2 &&
        TIM_CHANNEL_X != TIM_CHANNEL_3 && TIM_CHANNEL_X != TIM_CHANNEL_4)
    {
        Error_Handler();
    }

    pwm_handler.p_htim = p_htim;
    pwm_handler.timer_ch = TIM_CHANNEL_X;

    /* Start PWM */
    if (HAL_TIM_PWM_Start(p_htim, TIM_CHANNEL_X) != HAL_OK)
    {
        Error_Handler();
    }

    return pwm_handler;
}

/**
 *  @brief Set PWM duty cycle
 *  @param pointer to the pwm handler
 *  @param duty_cycle_percent is the duty cycle as
 *  	   percentage from 0-100
 */
void setDutyCycle(pwm_handler_t *p_pwm_handler, uint8_t duty_cycle_percent)
{
    /* Check for errors */
    if (p_pwm_handler->p_htim == NULL)
    {
        Error_Handler();
    }

    /* Check Duty Cycle is within a valid range */
    if (duty_cycle_percent < MIN_DUTY_CYCLE)
    {
        duty_cycle_percent = MIN_DUTY_CYCLE;
    }
    else if (duty_cycle_percent > MAX_DUTY_CYCLE)
    {
        duty_cycle_percent = MAX_DUTY_CYCLE;
    }

    /* Calculate Capture Compare value */
    float pulse = ((float)duty_cycle_percent / 100) *
                  (p_pwm_handler->p_htim->Instance->ARR + 1);

    uint16_t output_compare = (uint16_t)pulse;

    /* Set Output Compare Register to change duty cycle */
    __HAL_TIM_SET_COMPARE(p_pwm_handler->p_htim,
                          p_pwm_handler->timer_ch,
                          output_compare);
}
