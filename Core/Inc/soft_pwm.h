/**
 * @file soft_pwm.h
 */

/*
 *  Created on: 4 Feb 2022
 *      Author: niuslar
 */

#ifndef SOFT_PWM_H_
#define SOFT_PWM_H_

#include "main.h"

#define DUTY_STEPS 100

typedef struct
{
    GPIO_TypeDef *pwm_gpio_port;
    TIM_HandleTypeDef *p_htim;
    uint32_t *p_duty_cycle_buf;
    uint16_t pwm_pin;
} soft_pwm_handler_t;

/** @note These variables are "extern" becuase they are used
 *  in the interrupt handler
 */
extern volatile uint8_t duty_cycle_counter;

extern uint32_t GPIOA_duty_cycle_buf[DUTY_STEPS];
extern uint32_t GPIOB_duty_cycle_buf[DUTY_STEPS];
extern uint32_t GPIOC_duty_cycle_buf[DUTY_STEPS];

/* Exported Function Prototypes */
soft_pwm_handler_t startSoftPWM(GPIO_TypeDef *pwm_port,
                                uint16_t pwm_pin,
                                TIM_HandleTypeDef *p_htim);
void setSoftDutyCycle(soft_pwm_handler_t *p_soft_pwm_h,
                      uint8_t duty_cycle_percent);

#endif /* SOFT_PWM_H_ */
