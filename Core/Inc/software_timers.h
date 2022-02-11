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
#define MAX_SOFT_PWM_CHANNELS 5
#define DEFAULT_DUTY_CYCLE    0

/* uncomment if you need to allow true 100% duty cycle. */
#define FORCE_LIMITS
enum ERROR_CODES
{
    NO_ERROR = 0,
    ERROR_NULL_POINTER,
    ERROR_NOT_PIN,
    ERROR_MAX_CHANNEL_COUNT,
    ERROR_CALLBACK_FAIL,
    ERROR_START_FAIL
};

/* This structure essentially defines what GPIO port and pin the output should
 * be routed to. */
typedef struct
{
    GPIO_TypeDef *p_port;
    uint32_t pin;
    uint8_t duty_cycle;
} soft_pwm_handler_t;

typedef uint8_t soft_pwm_id_t;

uint8_t startSoftPwmTimer(TIM_HandleTypeDef *p_timer);

uint8_t registerSoftPwm(soft_pwm_id_t *p_soft_pwm_id,
                        GPIO_TypeDef *p_port,
                        uint32_t pin);
void setSoftPwmDutyCycle(soft_pwm_id_t soft_pwm_id, uint8_t duty_cycle_percent);

#endif /* SOFTWARE_TIMERS_H_ */
