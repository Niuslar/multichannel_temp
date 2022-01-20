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

#define PID_CHANNELS 		8
#define DEFAULT_SET_POINT	2500
#define ADC_RES	     		4096
#define ADC_VDDA     		(3.3)
#define HEAT_MODE 			0
#define COOL_MODE 			1
#define ADC_START_BIT 2

/**
 * @brief PID Channel configuration struct.
 */
typedef struct{
	TIM_HandleTypeDef* htim; /* Timer handle assigned to this channel */
	TIM_OC_InitTypeDef* sConfig; /* Timer output compare configuration struct */
	GPIO_TypeDef* heat_cool_port; /* Port of the heat_cool pin */
	uint32_t heat_cool_pin;
	uint32_t timer_channel; /* Timer channel to control the PWM of the main channel */
	uint8_t main_channel; /* Channel Number 0-7 */
}pid_channel_config_t;

/* Exported Functions Prototypes */
void PIDInit(TIM_HandleTypeDef* htim, uint8_t main_channel, pid_channel_config_t* pid_channel);
void PIDControl(uint16_t input, pid_channel_config_t* pid_channel, uint8_t adc_channel);
void setTargetTemp(float temp);

#endif /* INC_HEATERS_H_ */
