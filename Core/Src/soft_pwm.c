/**
 * @file soft_pwm.c
 *
 *  Created on: 4 Feb 2022
 *      Author: niuslar
 */

#include "soft_pwm.h"

#define MIN_DUTY_CYCLE 0
#define MAX_DUTY_CYCLE 100

/* Variables for interrupt handler */
volatile uint8_t duty_cycle_counter = 0;

/** @note If using a different MCU with more GPIO ports available,
 *  add more buffers here, in the function startSoftPWM() and in
 *  the interrupt handler
 */
uint32_t GPIOA_duty_cycle_buf[DUTY_STEPS] = {0};
uint32_t GPIOB_duty_cycle_buf[DUTY_STEPS] = {0};
uint32_t GPIOC_duty_cycle_buf[DUTY_STEPS] = {0};

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

/**
 * @brief start software PWM with a duty cycle of 0
 * @param pwm_gpio_port -> GPIO port of the pin used for soft PWM
 * @param pwm_pin -> GPIO Pin used for soft PWM
 * @retval soft_pwm_handler -> Initialised soft PWM Handler
 */
soft_pwm_handler_t startSoftPWM(GPIO_TypeDef *pwm_gpio_port,
                                uint16_t pwm_pin,
                                TIM_HandleTypeDef *p_htim)
{
    /* Check arguments for errors */
    if (pwm_gpio_port == NULL || p_htim == NULL)
    {
        Error_Handler();
    }

    soft_pwm_handler_t soft_pwm_handler;

    soft_pwm_handler.p_htim = p_htim;
    soft_pwm_handler.pwm_gpio_port = pwm_gpio_port;
    soft_pwm_handler.pwm_pin = pwm_pin;

    /* Assign right buffer depending on the GPIO port */
    if (pwm_gpio_port == GPIOA)
    {
        soft_pwm_handler.p_duty_cycle_buf = GPIOA_duty_cycle_buf;
    }
    else if (pwm_gpio_port == GPIOB)
    {
        soft_pwm_handler.p_duty_cycle_buf = GPIOB_duty_cycle_buf;
    }
    else if (pwm_gpio_port == GPIOC)
    {
        soft_pwm_handler.p_duty_cycle_buf = GPIOC_duty_cycle_buf;
    }

    /* Start timer with interrupt */
    HAL_TIM_Base_Start_IT(soft_pwm_handler.p_htim);

    return soft_pwm_handler;
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
            p_soft_pwm_h->p_duty_cycle_buf[step] &=
                ~(p_soft_pwm_h->pwm_pin << 16);
            p_soft_pwm_h->p_duty_cycle_buf[step] |= p_soft_pwm_h->pwm_pin;
        }
        else
        {
            /* Reset pin */
            p_soft_pwm_h->p_duty_cycle_buf[step] |=
                (p_soft_pwm_h->pwm_pin << 16);
            p_soft_pwm_h->p_duty_cycle_buf[step] &= ~p_soft_pwm_h->pwm_pin;
        }
    }
}
