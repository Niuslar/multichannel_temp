/**
 * @file pid_control.c
 * @brief PID Control functions
 */

/*  Created on: 11 Jan 2022
 *      Author: niuslar
 */


#include "pid_control.h"
#include "main.h"
#include "math.h"


/* Private variables */
static float kp, kp_cooler, kp_heater;
static float ki, ki_cooler, ki_heater;
static float kd, kd_cooler, kd_heater;
static int16_t error[PID_CHANNELS];
static uint16_t set_point = DEFAULT_SET_POINT;
static uint32_t output ;
static uint32_t previous_time[PID_CHANNELS];
static uint32_t elapsed_time_ms[PID_CHANNELS]; /* Elapsed time between each PIDControl() call*/
static uint32_t current_time_ms[PID_CHANNELS];
static float cum_error[PID_CHANNELS] = {0}; /* Cumulative Error */
static float deri_error[PID_CHANNELS] = {0}; /* Derivative of the error */
static int16_t previous_error[PID_CHANNELS] = {0}; /* Error from the previous reading */


/**
 * @ref TIM_OC_InitTypeDef
 */
static TIM_OC_InitTypeDef pwm_config = {
	.OCFastMode = TIM_OCFAST_DISABLE,
	.OCMode		= TIM_OCMODE_PWM1,
	.OCPolarity = TIM_OCPOLARITY_HIGH,
	.Pulse		= 0x0000
};


/**
 * @brief Set parameters and start PWM with a duty cycle of 0%
 * @param htim TIM handle
 * @param main_channel is the Control Channel (0-7)
 * @param pid_channel configuration struct to be filled
 * @return None
 */
void PIDInit(TIM_HandleTypeDef* htim, uint8_t main_channel, pid_channel_handle_t* pid_channel)
{
	/* Set pid_channel_config parameters */
	pid_channel->htim = htim;
	pid_channel->main_channel = main_channel;
	pid_channel->sConfig = &pwm_config;

	switch(main_channel){
	case 0:
		pid_channel->heat_cool_port = HEAT_COOL_1_GPIO_Port;
		pid_channel->heat_cool_pin	= HEAT_COOL_1_Pin;
		pid_channel->timer_channel = TIM_CHANNEL_1;
	case 1:
		pid_channel->heat_cool_port = HEAT_COOL_2_GPIO_Port;
		pid_channel->heat_cool_pin	= HEAT_COOL_2_Pin;
		pid_channel->timer_channel = TIM_CHANNEL_2;
	case 2:
		pid_channel->heat_cool_port = HEAT_COOL_3_GPIO_Port;
		pid_channel->heat_cool_pin	= HEAT_COOL_3_Pin;
		pid_channel->timer_channel = TIM_CHANNEL_3;
	case 3:
		pid_channel->heat_cool_port = HEAT_COOL_4_GPIO_Port;
		pid_channel->heat_cool_pin	= HEAT_COOL_4_Pin;
		pid_channel->timer_channel = TIM_CHANNEL_4;
	case 4:
		pid_channel->heat_cool_port = HEAT_COOL_5_GPIO_Port;
		pid_channel->heat_cool_pin	= HEAT_COOL_5_Pin;
		pid_channel->timer_channel = TIM_CHANNEL_1;
	case 5:
		pid_channel->heat_cool_port = HEAT_COOL_6_GPIO_Port;
		pid_channel->heat_cool_pin	= HEAT_COOL_6_Pin;
		pid_channel->timer_channel = TIM_CHANNEL_2;
	case 6:
		pid_channel->heat_cool_port = HEAT_COOL_7_GPIO_Port;
		pid_channel->heat_cool_pin	= HEAT_COOL_7_Pin;
		pid_channel->timer_channel = TIM_CHANNEL_1;
	case 7:
		pid_channel->heat_cool_port = HEAT_COOL_8_GPIO_Port;
		pid_channel->heat_cool_pin	= HEAT_COOL_8_Pin;
		pid_channel->timer_channel = TIM_CHANNEL_2;
	}

	/* Start PWM */
	HAL_TIM_PWM_Start(htim, pid_channel->timer_channel);
}

/**
 * @brief Calculate control output
 * @param ADC reading as input
 * @param pid_channel configuration struct
 * @param adc_channel where the reading comes from (0-7)
 * @return None
 */
void PIDControl(uint16_t input, pid_channel_handle_t* pid_channel, uint8_t adc_channel)
{
	current_time_ms[adc_channel] = HAL_GetTick();
	elapsed_time_ms[adc_channel] = (current_time_ms[adc_channel] - previous_time[adc_channel]);
	error[adc_channel] = set_point - input;

	/* If error is negative, then we need to use the heater */
	if(error[adc_channel] < 0)
	{
		HAL_GPIO_WritePin(pid_channel->heat_cool_port, pid_channel->heat_cool_pin, HEAT_MODE);
		error[adc_channel] = -error[adc_channel];
		kp = kp_heater;
		ki = ki_heater;
		kd = kd_heater;
	}
	else if(error[adc_channel] > 0)
	{
		HAL_GPIO_WritePin(pid_channel->heat_cool_port, pid_channel->heat_cool_pin, COOL_MODE);
		kp = kp_cooler;
		ki = ki_cooler;
		kd = kd_cooler;
	}

	cum_error[adc_channel] += error[adc_channel] * elapsed_time_ms[adc_channel];
	deri_error[adc_channel] = ((float)error[adc_channel] - previous_error[adc_channel])/(elapsed_time_ms[adc_channel]);
	output = kp * error[adc_channel] + ki * cum_error[adc_channel] + kd * deri_error[adc_channel];
	/* Parameters for next cycle */
	previous_error[adc_channel] = error[adc_channel];
	previous_time[adc_channel] = current_time_ms[adc_channel];

	/* Control PWM */
	/* Because of the timers configuration, the PWM signal frequency is 10KHz
	 * The duty cycle is calculated as: Pulse/Counter Period
	 * The counter period is 3200
	 */
	pid_channel->sConfig->Pulse = output;
	HAL_TIM_PWM_ConfigChannel(pid_channel->htim, pid_channel->sConfig, pid_channel->timer_channel);

}

/**
 * @brief Set the target temperature for the system
 * @param temperature in celsius
 * @return None
 */
void setTargetTemp(float temp)
{
	/* First convert celsius to voltage using a polynomial approximation */
	/* 2.85 + -8.29E-03x + -1.89E-04x^2 + 9.5E-07x^3 */
	float voltage = 2.85 - 0.00829 * temp - 0.000189 * pow(temp,2) + 0.00000095 * pow(temp,3);

	/* Then transform voltage to digital value */
	uint16_t digital_value = ((float)voltage/ADC_VDDA)*ADC_RES;

	set_point = digital_value;
}

/**
 * @brief Disable Heaters and Coolers in case of temperature error
 * param Pointer to PID channels handle array
 * @return None
 */
void disablePID(pid_channel_handle_t* pid_channel)
{
	for(uint8_t channel = 0; channel < PID_CHANNELS; channel++)
	{
		HAL_TIM_PWM_Stop(pid_channel[channel].htim, pid_channel[channel].timer_channel);
	}
}





