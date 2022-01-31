/**
 * @file timers.c
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */

#include "timers.h"

/** All timers start with a counter period of 3200
 * 	which translates to a frequency of 10,000 KHz.
 * 	Use the function setFrequency to change the frequency of a timer.
 */
#define CLOCK_SOURCE_KHZ       32000
#define DEFAULT_COUNTER_PERIOD 3200
#define MIN_DUTY_CYCLE         0
#define MAX_DUTY_CYCLE         100
#define MIN_FREQ               1  // TODO: Find actual min and max for the frequency
#define MAX_FREQ               100

/* Private variables */
static pwm_handler_t pwm_channels[CONTROL_CHANNELS];

/* Private functions */
static void startSoftPWM(uint8_t control_channel);

/**
 * @brief Save configuration for control channel
 * @param control channel from 1-CONTROL_CHANNELS
 * @param timer channel (1-4 for timer 2, 1-2 for timers 21 and 22)
 * @param htim	pointer to timer handler, if using a soft time (channels 9-10)
 * set pointer as NULL.
 */
void configCtrlChannel(uint8_t control_channel,
                       uint8_t timer_channel,
                       TIM_HandleTypeDef *p_htim)
{
    pwm_channels[control_channel - 1].control_ch = control_channel;
    pwm_channels[control_channel - 1].counter_period = DEFAULT_COUNTER_PERIOD;

    /* Check if error or software timer */
    if (p_htim == NULL && (control_channel == 9 || control_channel == 10))
    {
        startSoftPWM(control_channel);
        return;
    }
    else if (p_htim == NULL)
    {
        Error_Handler();
    }
    pwm_channels[control_channel - 1].p_htim = p_htim;

    switch (timer_channel)
    {
        case 1:
            pwm_channels[control_channel - 1].timer_ch = TIM_CHANNEL_1;
        case 2:
            pwm_channels[control_channel - 1].timer_ch = TIM_CHANNEL_2;
        case 3:
            pwm_channels[control_channel - 1].timer_ch = TIM_CHANNEL_3;
        case 4:
            pwm_channels[control_channel - 1].timer_ch = TIM_CHANNEL_4;
    }
}

/**
 *  @brief Set PWM duty cycle
 *  @param p_pwm_handler pointer to pwm handler of the corresponding channel
 *  @param duty_cycle is the duty cycle between 1-100
 */
void setDutyCycle(uint8_t control_channel, uint8_t duty_cycle)
{
    /* Check it's a valid control channel */
    if (control_channel < 0 || control_channel > CONTROL_CHANNELS)
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
                   pwm_channels[control_channel - 1].counter_period);
    uint16_t output_compare = (uint16_t)pulse - 1;

    /* Check if the control channel is soft or hard PWM */
    if (control_channel < 8)
    {
        /* Set Output Compare Register to change duty cycle */
        __HAL_TIM_SET_COMPARE(pwm_channels[control_channel - 1].p_htim,
                              pwm_channels[control_channel - 1].timer_ch,
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
 *  @param p_htim pointer to timer handler
 *  @param freq_khz frequency in khz
 */
void setFrequency(TIM_HandleTypeDef *p_htim, uint16_t freq_khz)
{
    if (p_htim == NULL)
    {
        Error_Handler();
    }
    /* Check frequency is within range */
    if (freq_khz < MIN_FREQ || freq_khz > MAX_FREQ)
    {
        Error_Handler();
    };

    /* Clock source: 32 MHz
     * Counter period = Clock source / Frequency
     */
    float counter_float = ((float)CLOCK_SOURCE_KHZ / freq_khz);
    uint16_t counter_period = (uint16_t)counter_float;

    /* Change timer counter period */
    __HAL_TIM_SET_COUNTER(p_htim, counter_period);

    /* Update the timer period in the control channels */
    for (uint8_t channel; channel < CONTROL_CHANNELS; channel++)
    {
        if (pwm_channels[channel - 1].p_htim == p_htim)
        {
            pwm_channels[channel - 1].counter_period = counter_period;
        }
    }
}

/* TODO: Complete function */
void startSoftPWM(uint8_t control_channel)
{
    /* Only channels 9 and above should use software PWM */
}
