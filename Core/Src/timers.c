/**
 * @file timers.c
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */

#include "timers.h"

/** All timers start with a counter period of 3200
 * 	which translates to a frequency of 10,000Hz.
 * 	Use the function setFrequency to change the frequency of a timer.
 */
#define CLOCK_SOURCE_HZ 32000000
#define MIN_DUTY_CYCLE  0
#define MAX_DUTY_CYCLE  100

/** @note These Min and Max frequencies are based on a
 * 32MHz clock source with no prescaler.
 * The Max frequency is calculated in order to keep
 * at least an 8-bit resolution for the duty cycle.
 */
#define MIN_HW_FREQ_HZ 489
#define MAX_HW_FREQ_HZ 125000

// TODO: Find actual min and max values
// for softPWM

/** When using software PWM, the MAX and MIN
 * frequencies will change
 */
#define MIN_SW_FREQ_HZ 1
#define MAX_SW_FREQ_HZ 1250

/* Private variables */
/** This variable stores the values to be copied into BSRR */
uint32_t soft_pwm_duty_buf[DUTY_STEPS] = {0};

/* Private functions */
static void startSoftPWM(pwm_handler_t *p_pwm_handler);

/**
 * @brief Save configuration for control channel and start PWM
 * @param pointer to pwm handler
 * @retval none
 */
void startPWM(pwm_handler_t *p_pwm_handler)
{
    /* Check for errors */
    if (p_pwm_handler->control_ch < 0 ||
        p_pwm_handler->control_ch >= CONTROL_CHANNELS ||
        p_pwm_handler->p_htim == NULL)
    {
        Error_Handler();
    }

    /* Check if software timer */
    if (p_pwm_handler->control_ch == CONTROL_CH_9 ||
        p_pwm_handler->control_ch == CONTROL_CH_10)
    {
        startSoftPWM(p_pwm_handler);
        return;
    }

    /* Start PWM with default config */
    HAL_TIM_PWM_Start(p_pwm_handler->p_htim, p_pwm_handler->timer_ch);
}

// TODO: Add a variable that holds the previous duty cycle and compare
// steps.

/**
 *  @brief Set PWM duty cycle
 *  @note Make sure the frequency configuration is set before calling this
 * function
 *  @param control channel from 0 to CONTROL_CHANNELS-1
 *  @param duty_cycle is the duty cycle between 1-100
 */
void setDutyCycle(pwm_handler_t *p_pwm_handler, uint8_t duty_cycle)
{
    /* Check for errors */
    if (p_pwm_handler->control_ch < 0 ||
        p_pwm_handler->control_ch >= CONTROL_CHANNELS ||
        p_pwm_handler->p_htim == NULL)
    {
        Error_Handler();
    }

    /* Check Duty Cycle is within a valid range */
    if (duty_cycle < MIN_DUTY_CYCLE || duty_cycle > MAX_DUTY_CYCLE)
    {
        Error_Handler();
    }

    /* Check if the control channel is soft or hard PWM */
    if (p_pwm_handler->control_ch < CONTROL_CH_9)
    {
        /* Calculate Capture Compare value */
        float pulse = ((float)duty_cycle / 100) *
                      (p_pwm_handler->p_htim->Instance->ARR + 1);

        uint16_t output_compare = (uint16_t)pulse - 1;

        /* Set Output Compare Register to change duty cycle */
        __HAL_TIM_SET_COMPARE(p_pwm_handler->p_htim,
                              p_pwm_handler->timer_ch,
                              output_compare);
    }

    else if (p_pwm_handler->control_ch < CONTROL_CHANNELS)
    {
        /* To change the duty cycle of a software timer, we need to change each
         * "Duty Step".
         */
        for (uint8_t step = 0; step < DUTY_STEPS; step++)
        {
            if (step < duty_cycle)
            {
                /* Set the pin */
                soft_pwm_duty_buf[step] &=
                    ~(uint32_t)(p_pwm_handler->control_pin << 16);
                soft_pwm_duty_buf[step] |= (uint32_t)p_pwm_handler->control_pin;
            }
            else
            {
                /* Reset pin */
                soft_pwm_duty_buf[step] |=
                    (uint32_t)(p_pwm_handler->control_pin << 16);
                soft_pwm_duty_buf[step] &=
                    ~(uint32_t)p_pwm_handler->control_pin;
            }
        }
    }
}

/**
 *  @brief Change frequency of the selected channel
 *  @note This function changes the frequency of the timer related to the
 *  specified channel and all control channels that use that timer will be
 *  affected.
 *  @param p_htim pointer to timer handler
 *  @param freq_hz frequency in Hz
 */
void setFrequency(pwm_handler_t *p_pwm_handler, uint32_t freq_hz)
{
    if (p_pwm_handler == NULL)
    {
        Error_Handler();
    }

    /* Check if the control channel uses softPWM */
    if (p_pwm_handler->control_ch == CONTROL_CH_9 ||
        p_pwm_handler->control_ch == CONTROL_CH_10)
    {
        /* Software timer -> Freq needs to be multiplied by the number
         * of duty steps used. Each timer interrupt updates 1 step
         */
        freq_hz *= DUTY_STEPS;
    }

    /* Check frequency is within range */
    if (freq_hz < MIN_HW_FREQ_HZ || freq_hz > MAX_HW_FREQ_HZ)
    {
        Error_Handler();
    };

    /* Clock source: 32 MHz, No prescaler
     * Counter period = Clock source / Frequency
     */
    float counter_float = ((float)CLOCK_SOURCE_HZ / freq_hz);
    uint16_t counter_period = (uint16_t)counter_float - 1;

    /* Change timer counter period */
    __HAL_TIM_SET_AUTORELOAD(p_pwm_handler->p_htim, counter_period);
}

static void startSoftPWM(pwm_handler_t *p_pwm_handler)
{
    /* Start timer with interrupt */
    HAL_TIM_Base_Start_IT(p_pwm_handler->p_htim);
}

uint32_t *getDutyCycle()
{
    return soft_pwm_duty_buf;
}
