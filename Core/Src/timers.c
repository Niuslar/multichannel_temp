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
#define CLOCK_SOURCE_HZ        32000000
#define DEFAULT_COUNTER_PERIOD 3200
#define MIN_DUTY_CYCLE         0
#define MAX_DUTY_CYCLE         100

/** @note These Min and Max frequencies are based on a
 * 32MHz clock source with no prescaler.
 * The Max frequency is calculated in order to keep
 * at least an 8-bit resolution for the duty cycle.
 */
#define MIN_FREQ_HZ 489
#define MAX_FREQ_HZ 125000

/* Private variables */
static pwm_handler_t pwm_channels[CONTROL_CHANNELS];

/* Private functions */
static void startSoftPWM(uint8_t control_channel);

/**
 * @brief Save configuration for control channel and start PWM
 * @param control channel in the form of CONTROL_CH_X (X from 1 to 10)
 * @param timer channel as TIM_CHANNEL_X (X from 1 to 4)
 * @param htim	pointer to timer handler, if using a soft time (channels 9-10)
 * set pointer as NULL.
 */
void startPWM(control_ch_t control_channel,
              uint8_t timer_channel,
              TIM_HandleTypeDef *p_htim)
{
    if (control_channel < 0 || control_channel >= CONTROL_CHANNELS)
    {
        Error_Handler();
    }
    pwm_channels[control_channel].control_ch = control_channel;
    pwm_channels[control_channel].counter_period = DEFAULT_COUNTER_PERIOD;

    /* Check if software timer */
    if (p_htim == NULL && (control_channel == 8 || control_channel == 9))
    {
        startSoftPWM(control_channel);
        return;
    }

    /* Error if there's a NULL pointer with a hardware timer */
    else if (p_htim == NULL)
    {
        Error_Handler();
    }

    pwm_channels[control_channel].p_htim = p_htim;

    pwm_channels[control_channel].timer_ch = timer_channel;

    /* Start PWM with default config */
    HAL_TIM_PWM_Start(p_htim, pwm_channels[control_channel].timer_ch);
}

/**
 *  @brief Set PWM duty cycle
 *  @note Make sure the frequency configuration is set before calling this
 * function
 *  @param control channel from 0 to CONTROL_CHANNELS-1
 *  @param duty_cycle is the duty cycle between 1-100
 */
void setDutyCycle(control_ch_t control_channel, uint8_t duty_cycle)
{
    /* Check it's a valid control channel */
    if (control_channel < 0 || control_channel >= CONTROL_CHANNELS)
    {
        Error_Handler();
    }
    /* Check Duty Cycle is within a valid range */
    if (duty_cycle < MIN_DUTY_CYCLE || duty_cycle > MAX_DUTY_CYCLE)
    {
        Error_Handler();
    }

    /* Calculate Capture Compare value */
    float pulse = (((float)duty_cycle / 100) *
                   (pwm_channels[control_channel].counter_period + 1));
    uint16_t output_compare = (uint16_t)pulse - 1;

    /* Check if the control channel is soft or hard PWM */
    if (control_channel < 8)
    {
        /* Set Output Compare Register to change duty cycle */
        __HAL_TIM_SET_COMPARE(pwm_channels[control_channel].p_htim,
                              pwm_channels[control_channel].timer_ch,
                              output_compare);
    }

    else if (control_channel < 10)
    {
        /* Change soft pwm */
        // TODO: Add software timers
    }
}

/**
 *  @brief Change frequency of the selected timer
 *  @note Start all the control channels for the selected timer with startPWM()
 *  before calling this function.
 *  @param p_htim pointer to timer handler
 *  @param freq_hz frequency in Hz
 */
void setFrequency(TIM_HandleTypeDef *p_htim, uint32_t freq_hz)
{
    if (p_htim == NULL)
    {
        Error_Handler();
    }
    /* Check frequency is within range */
    if (freq_hz < MIN_FREQ_HZ || freq_hz > MAX_FREQ_HZ)
    {
        Error_Handler();
    };

    /* Clock source: 32 MHz, No prescaler
     * Counter period = Clock source / Frequency
     */
    float counter_float = ((float)CLOCK_SOURCE_HZ / freq_hz);
    uint16_t counter_period = (uint16_t)counter_float - 1;

    /* Change timer counter period */
    __HAL_TIM_SET_AUTORELOAD(p_htim, counter_period);

    /* Update the timer period in the control channels */
    for (uint8_t channel = 0; channel < CONTROL_CHANNELS; channel++)
    {
        if (pwm_channels[channel].p_htim == p_htim)
        {
            pwm_channels[channel].counter_period = counter_period;
        }
    }
}

/* TODO: Complete function */
void startSoftPWM(uint8_t control_channel)
{
    /* Only channels 9 and 10 should use software PWM */
}
