/**
 * @file pid_control.h
 * @brief PID Control header file
 */

/*  Created on: 11 Jan 2022
 *      Author: niuslar
 */

#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_

// this file should contain only things that you want visible from outside the module.

// uncomment this line if you want PID loop to perform limit checking of variables.
//#define LIMIT_CHECKING

/**
 * @brief PID Handler
 *
 * This structure contains all information for a specific PID loop to function.
 * For each instance of the loop, declare a single structure and then call
 * initialisation function for it.
 */
typedef struct{
	float kp;
	float ki;
	float kd;
#ifdef LIMIT_CHECKING
	float max_p;
	float min_p;
	float max_i;
	float min_i;
	float max_d;
	float min_d;
	float max_out;
	float min_out;
#endif
	float old_output;
	float old_error;
	float old_integral;
}pid_handle_t;

/* Exported Functions Prototypes */

/**
 * @brief Initialise newly created PID control structure.
 *
 * This will also reset all parameters to default values except control
 * coefficients.
 *
 * @param p_pid_handler Pointer to the PID handler data structure.
 */
void initPID(pid_handle_t* p_pid_handler);

/**
 * @brief Reset PID control to freshly initialised state.
 *
 * @param p_pid_handler Pointer to the PID handler data structure.
 */
void resetPID(pid_handle_t* p_pid_handler);

/**
 * @brief Run PID loop once and calculate new output.
 *
 * @param p_pid_handler Pointer to the PID handler data structure.
 * @param target_value Target value for the control variable.
 * @param actual_value Actual value of the control variable.
 *
 * @return New output of the PID loop.
 */
float runPID(pid_handle_t* p_pid_handler, float target_value, float actual_value);

#endif /* PID_CONTROL_H_ */
