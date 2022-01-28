/**
 * @file timers.c
 */

/*
 *  Created on: Jan 28, 2022
 *      Author: niuslar
 */


#include "timers.h"

#define COUNTER_PERIOD 	32
#define MIN_DC			0
#define MAX_DC			100
#define MIN_FREQ		1		// TODO: Find actual min and max for the frequency
#define MAX_FREQ		100

/**
 *  @brief Set PWM duty cycle
 *  @param p_pwm_handler pointer to pwm handler of the corresponding channel
 *  @param duty_cycle is the duty cycle between 1-100
 */

void setDutyCycle(pwm_handler_t* p_pwm_handler, uint8_t duty_cycle)
{
	if(p_pwm_handler == NULL)
	{
		Error_Handler();
	}
	/* Check for errors and convert duty cycle to pulse */
	if(duty_cycle < MIN_DC || duty_cycle > MAX_DC)
	{
		Error_Handler();
	}

	//TODO: Rewrite equation to consider other Counter Period values
	uint16_t pulse = duty_cycle * COUNTER_PERIOD;

	/* Check if the control channel is soft or hard PWM */
	if(p_pwm_handler->control_ch < 8)
	{
		/* Set Output Compare Register to change duty cycle */
		__HAL_TIM_SET_COMPARE(p_pwm_handler->htim, p_pwm_handler->timer_ch, pulse);
	}

	else if(p_pwm_handler->control_ch < 10)
	{
		/* Change soft pwm */
		//TODO: Add software timers

	}
}

void setFrequency(pwm_handler_t* p_pwm_handler, uint16_t freq_khz)
{
	if(p_pwm_handler == NULL)
	{
		Error_Handler();
	}
	/* Check for errors and convert duty cycle to pulse */
	if(freq_khz < MIN_FREQ || freq_khz > MAX_FREQ)
	{
		Error_Handler();
	}

	//TODO: Find equation
	uint16_t counter_value = 0;

	//TODO: Add filter for hard/softPWM
	__HAL_TIM_SET_COUNTER(p_pwm_handler->htim, counter_value);
}
