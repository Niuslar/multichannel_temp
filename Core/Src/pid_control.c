/*
 * @file pid_control.c
 *
 */

/*  Created on: 11 Jan 2022
 *      Author: niuslar
 */


#include "pid_control.h"
#include "main.h"

//Private variables
static uint16_t kp, kd, ki;
static uint16_t error;
static uint16_t set_point = DEFAULT_SET_POINT;
static uint16_t output ;
static uint32_t previous_time = 0;
static uint32_t elapsed_time = 0; /* Elapsed time between each PIDControl() call */
static uint32_t current_time = 0;
static uint32_t cum_error = 0; /* Cumulative Error */
static uint32_t deri_error = 0; /* Derivative of the error */
static float previous_error = 0; /* Error from the previous reading */

/**
 * @brief Set parameters to start PWM
 * @param None
 * @return None
 */
void PIDInit()
{
}

/**
 * @brief Calculate control output
 * @param input is ADC reading
 * @return output value
 */
uint16_t PIDControl(uint16_t input)
{
	current_time = HAL_GetTick();
	elapsed_time = current_time - previous_time;
	error = set_point - input;
	cum_error += error * elapsed_time;
	deri_error = (error - previous_error)/(elapsed_time);
	output = kp * error + ki * cum_error + kd * deri_error;

	/* parameters for next cycle */
	previous_error = error;
	previous_time = current_time;

	return output;
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
	uint16_t digital_value = (uint16_t)(voltage/ADC_VDDA)*ADC_RES;

	set_point = digital_value;
}
