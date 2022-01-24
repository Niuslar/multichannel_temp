/**
 * @file pid_control.h
 * @brief PID Control header file
 */

/*  Created on: 11 Jan 2022
 *      Author: niuslar
 */

#ifndef INC_HEATERS_H_
#define INC_HEATERS_H_

#include "main.h"

#define DEFAULT_TARGET  200
#define DEFAULT_KP		1
#define DEFAULT_KI		0
#define DEFAULT_KD		0

/**
 * @brief PID Handler
 */
typedef struct{
	float kp;
	float ki;
	float kd;
	float set_point;
	float prev_output;
	float prev_error;
	float cum_error;
	uint32_t previous_time;
}pid_handle_t;

/* Exported Functions Prototypes */
void PIDInit(pid_handle_t* pid_handler);
float PIDControlOutput(pid_handle_t* pid_handler, float input);
void setPIDTarget(pid_handle_t* pid_handler, float target);

#endif /* INC_HEATERS_H_ */
