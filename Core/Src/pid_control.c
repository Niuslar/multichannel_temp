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

static uint32_t current_time_ms;
static uint32_t elapsed_time_ms;
static float error;
static float deri_error;
static float output;


/**
 * @brief Initialise struct parameters with default values
 * @note Call only once at the beginning of the application
 * @param  pointer to pid_handler
 * @return None
 */
void PIDInit(pid_handle_t* pid_handler)
{
	pid_handler->cum_error = 0;
	pid_handler->prev_error = 0;
	pid_handler->kd = DEFAULT_KD;
	pid_handler->ki = DEFAULT_KI;
	pid_handler->kp = DEFAULT_KP;
	pid_handler->prev_output = 0;
	pid_handler->previous_time = 0;
	pid_handler->set_point = DEFAULT_TARGET;
}

/**
 * @brief Calculate PID control output
 * @param pointer to pid_handler
 * @param input value
 * @return output value
 */
float PIDControlOutput(pid_handle_t* pid_handler, float input)
{
	error = pid_handler->set_point - input;
	current_time_ms = HAL_GetTick();
	elapsed_time_ms = current_time_ms - pid_handler->previous_time;
	pid_handler->cum_error += error * elapsed_time_ms;
	deri_error = (error - pid_handler->prev_error)/elapsed_time_ms;

	output = (pid_handler->kp * error) + (pid_handler->ki * pid_handler->cum_error) + (pid_handler->kd * deri_error);

	/* Save values for next time */
	pid_handler->previous_time = current_time_ms;
	pid_handler->prev_output = output;
	pid_handler->prev_error = error;

	return output;
}


/**
 * @brief Set the target value for PID Control
 * @param pointer to pid_handler
 * @param target value
 * @return None
 */
void setPIDTarget(pid_handle_t* pid_handler, float target)
{
	pid_handler->set_point = target;
}



