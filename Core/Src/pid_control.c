/**
 * @file pid_control.c
 * @brief PID Control functions
 */

/*  Created on: 11 Jan 2022
 *      Author: niuslar
 */


#include "pid_control.h"
#include "math.h"
#include <float.h>

// these values don't need to be visible from outside, so moved them to .c
#define DEFAULT_KP		1
#define DEFAULT_KI		0
#define DEFAULT_KD		0

/* local PFP. */
float checkLimits(float value, float max_limit, float min_limit);

// doxygen comments should live in a .h file, not .c file. This is purely implementation.
void initPID(pid_handle_t* p_pid_handler)
{
	p_pid_handler->kd = DEFAULT_KD;
	p_pid_handler->ki = DEFAULT_KI;
	p_pid_handler->kp = DEFAULT_KP;
	//conditional compilation that can be toggled if necessary
#ifdef LIMIT_CHECKING
	p_pid_handler->max_p = FLT_MAX;
	p_pid_handler->min_p = -FLT_MAX;
	p_pid_handler->max_i = FLT_MAX;
	p_pid_handler->min_i = -FLT_MAX;
	p_pid_handler->max_d = FLT_MAX;
	p_pid_handler->min_d = -FLT_MAX;;
	p_pid_handler->max_out = FLT_MAX;
	p_pid_handler->min_out = -FLT_MAX;
#endif
	resetPID(p_pid_handler);
}


void resetPID(pid_handle_t* p_pid_handler)
{
	p_pid_handler->old_output = 0;
	p_pid_handler->old_error = 0;
	p_pid_handler->old_integral = 0;
}


float runPID(pid_handle_t* p_pid_handler, float target_value, float actual_value)
{
	//input sanitisation
	if(p_pid_handler == NULL)
	{
		return 0;
	}

	float output = p_pid_handler->old_integral;
	float pid_value;
	float error = target_value - actual_value;

	/* proportional coefficient */
	if(p_pid_handler->kp != 0)
	{
		pid_value = error * p_pid_handler->kp;
#ifdef LIMIT_CHECKING
		pid_value = checkLimits(pid_value, p_pid_handler->max_p, p_pid_handler->min_p);
#endif
		output += pid_value;
	}

	/* differential coefficient */
	if(p_pid_handler->kd != 0)
	{
		/*for now we assume that this loop is called at regular intervals, so
		 * no timing is necessary. */
		pid_value = (error - p_pid_handler->old_error) * p_pid_handler->ki;
#ifdef LIMIT_CHECKING
		pid_value = checkLimits(pid_value, p_pid_handler->max_d, p_pid_handler->min_d);
#endif
		output += pid_value;
	}

	/* integral coefficient. this is done somewhat out of regular sequence to
	 * facilitate limit checking of the integral accumulator and output. */
	if(p_pid_handler->ki != 0)
	{
		/*for now we assume that this loop is called at regular intervals, so
		 * no timing is necessary. */
		pid_value = error * p_pid_handler->ki;
#ifdef LIMIT_CHECKING
		pid_value = checkLimits(pid_value, p_pid_handler->max_i, p_pid_handler->min_i);
#endif
		output += pid_value;

#ifdef LIMIT_CHECKING
		/* If output is saturated at max or min limits, then integral
		 * accumulator should be prevented from incrementing/decrementing. */
		if(output > p_pid_handler->max_out){
			output = p_pid_handler->max_out;
			/* since output is saturated at max value, prevent integral from increasing.*/
			if(pid_value > 0)
			{
				pid_value = 0;
			}
		}
		if(output < p_pid_handler->min_out){
			output = p_pid_handler->min_out;
			/* since output is saturated at min value, prevent integral from decreasing.*/
			if(pid_value < 0)
			{
				pid_value = 0;
			}
		}
#endif
	}
	p_pid_handler->old_error = error;
	p_pid_handler->old_integral += pid_value;
	p_pid_handler->old_output = output;
	return output;
}


#ifdef LIMIT_CHECKING
float checkLimits(float value, float max_limit, float min_limit)
{
	if(value > max_limit)
	{
		value = max_limit;
	}
	if(value < min_limit)
	{
		value = min_limit;
	}
	return value;
}
#endif

