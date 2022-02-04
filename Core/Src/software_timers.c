/*
 * software_timers.c
 *
 *  Created on: 4 Feb 2022
 *      Author: niuslar
 */

#include "software_timers.h"

#define MIN_DUTY_CYCLE 0
#define MAX_DUTY_CYCLE 100

/**
 * @note To simulate the duty cycle of a hardware timer,
 * software timers check the status of the pin and a counter
 * with each interrupt. The status will be high when
 * counter < duty_cycle and low otherwise.
 *
 * In this case we use 100 "duty steps" to control the timers.
 * This means the counter goes from 0 to 100 and it restarts.
 * It also means that the PWM frequency is:
 *
 *  PWM Freq = Timer Freq. / 100;
 *
 *  Which for TIM6 is:
 *
 *  PWM Freq = 5000 / 100 = 50 Hz;
 *
 *  To change this frequency, change the counter period
 *  in the Device Configuration Tool.
 */

/* Private Variables */
uint16_t soft_pwm_duty_buf[DUTY_STEPS] = {0};

/**
 * @brief start software PWM with a duty cycle of 0
 * @param pointer to soft pwm handler
 * @retval None
 */
void startSoftPWM(soft_pwm_handler_t *p_soft_pwm_h)
{
    /* Start Clock */
    /* Start timer with interrupt */
    HAL_TIM_Base_Start_IT(p_soft_pwm_h->p_htim);
}

/**
 * @brief set duty cycle for selected software timer
 * @param pointer to soft pwm handler
 * @retval None
 */
void setSoftDutyCycle(soft_pwm_handler_t *p_soft_pwm_h,
                      uint8_t duty_cycle_percent)
{
    /* Check Duty Cycle is within a valid range */
    if (duty_cycle_percent < MIN_DUTY_CYCLE)
    {
        duty_cycle_percent = MIN_DUTY_CYCLE;
    }
    else if (duty_cycle_percent > MAX_DUTY_CYCLE)
    {
        duty_cycle_percent = MAX_DUTY_CYCLE;
    }

    /* Update buffer with the right values */
    for (uint8_t step = 0; step < DUTY_STEPS; step++)
    {
        if (step < duty_cycle_percent)
        {
            /* Set the pin */
            p_soft_pwm_h->p_duty_cycle_buf[step] |= p_soft_pwm_h->control_pin;
        }
        else
        {
            /* Reset pin */
            p_soft_pwm_h->p_duty_cycle_buf[step] &= ~p_soft_pwm_h->control_pin;
        }
    }
}
