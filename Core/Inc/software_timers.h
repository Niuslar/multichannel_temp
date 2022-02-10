/**
 * @file software_timers.h
 */

/*
 *  Created on: 4 Feb 2022
 *      Author: niuslar
 */

#ifndef SOFTWARE_TIMERS_H_
#define SOFTWARE_TIMERS_H_

#include "main.h"

#define DUTY_STEPS            100
#define MAX_SOFT_PWM_CHANNELS 5

enum ERROR_CODES
{
    NO_ERROR = 0,
    ERROR_NULL_POINTER,
    ERROR_NOT_PIN,
    ERROR_MAX_CHANNEL_COUNT
};

/* This structure essentially defines what GPIO port and pin the output should
 * be routed to. */
typedef struct
{
    GPIO_TypeDef *p_port;
    uint32_t pin;
} soft_pwm_handler_t;

uint8_t startSoftPWMTimer(TIM_HandleTypeDef *p_timer);
uint8_t registerSoftPWM(soft_pwm_handler_t *p_soft_pwm_handler,
                        GPIO_TypeDef *p_port,
                        uint32_t pin);
void setSoftDutyCycle(soft_pwm_handler_t *p_soft_pwm_handler,
                      uint8_t duty_cycle_percent);
uint16_t *getDutyCycle();

#endif /* SOFTWARE_TIMERS_H_ */
