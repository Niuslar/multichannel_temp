/*
 * software_timers.c
 *
 *  Created on: 4 Feb 2022
 *      Author: niuslar
 */

#include "software_timers.h"

#define MIN_DUTY_CYCLE 0
#define MAX_DUTY_CYCLE 100

static TIM_HandleTypeDef *p_software_timer;
static soft_pwm_handler_t registered_soft_pwm[MAX_SOFT_PWM_CHANNELS];
static uint8_t soft_pwm_counter = 0;

void tickSoftPWM(TIM_HandleTypeDef *htim);

/**
 * @note To simulate the duty cycle of a hardware timer,
 * software timers check the status of the pin and a counter
 * with each interrupt. The status will be high when
 * counter < duty_cycle and low otherwise.
 *
 * In this case we use 100 "duty steps" to control the timers.
 * This means the counter goes from 0 to 100 and it restarts.
 * It also means that the PWM frequency is:
 *
 *  PWM Freq = Timer Freq. / 100;
 *
 *  Which for TIM6 is:
 *
 *  PWM Freq = 5000 / 100 = 50 Hz;
 *
 *  To change this frequency, change the counter period
 *  in the Device Configuration Tool.
 */

/**
 * @brief Initialise the timer that will generate interrupts for software PWM.
 * @note  It is assumed that only one timer is responsible for running all soft
 * PWM outputs.
 *
 * @param p_timer Pointer to timer handler.
 * @return NULL if success, error code otherwise.
 */
uint8_t startSoftPWMTimer(TIM_HandleTypeDef *p_timer)
{
    if (p_timer == NULL)
    {
        return ERROR_NULL_POINTER;
    }
    /* It is possible to completely reorganise the timer here by following the
     * format of CubeMX timers setup. Specifically, it is possible to change
     * prescaler, Period, etc. It would also be trivial to change interrupt
     * behaviour of the timer.
     * Whether this is necessary or user should be responsible for setting up
     * the timer remains to be seen.
     */
    p_software_timer = p_timer;
    /* Now need to register a callback to trigger on period elapse event and
     * start timer.*/
    if (HAL_TIM_RegisterCallback(p_timer,
                                 HAL_TIM_PERIOD_ELAPSED_CB_ID,
                                 tickSoftPWM) != HAL_OK)
    {
        return ERROR_CALLBACK_FAIL;
    }

    if(HAL_TIM_Base_Start_IT(p_software_timer)!= HAL_OK)
    	{
    	return ERROR_START_FAIL;
    	}
    return NO_ERROR;
}

/**
 * @brief Register GPIO port and pin as software PWM.
 *
 * @param p_pwm_handler Pointer to a software PWM handler that will contain
 * registered pin.
 * @param p_port GPIO port.
 * @param pin GPIO pin.
 * @return NULL if success, error code otherwise.
 */
uint8_t registerSoftPWM(soft_pwm_handler_t *p_pwm_handler,
                        GPIO_TypeDef *p_port,
                        uint32_t pin)
{
    if ((p_pwm_handler == NULL) || (p_port == NULL))
    {
        return ERROR_NULL_POINTER;
    }
    if (!IS_GPIO_PIN(pin))
    {
        return ERROR_NOT_PIN;
    }
    if (soft_pwm_counter >= MAX_SOFT_PWM_CHANNELS)
    {
        return ERROR_MAX_CHANNEL_COUNT;
    }

    // TODO: check if returning the pointer to external entity is even
    // necessary.
    p_pwm_handler->p_port = p_port;
    p_pwm_handler->pin = pin;
    registered_soft_pwm[soft_pwm_counter].p_port = p_port;
    registered_soft_pwm[soft_pwm_counter].pin = pin;

    soft_pwm_counter++;
    return NO_ERROR;
}

/**
 * @brief set duty cycle for selected software timer
 * @param pointer to soft pwm handler
 * @retval None
 */
void setSoftDutyCycle(soft_pwm_handler_t *p_soft_pwm_h,
                      uint8_t duty_cycle_percent)
{
    /* Check Duty Cycle is within a valid range */
    if (duty_cycle_percent < MIN_DUTY_CYCLE)
    {
        duty_cycle_percent = MIN_DUTY_CYCLE;
    }
    else if (duty_cycle_percent > MAX_DUTY_CYCLE)
    {
        duty_cycle_percent = MAX_DUTY_CYCLE;
    }

    /* Update buffer with the right values */
//    for (uint8_t step = 0; step < DUTY_STEPS; step++)
//    {
//        if (step < duty_cycle_percent)
//        {
//            /* Set the pin */
//            p_soft_pwm_h->p_duty_cycle_buf[step] &=
//                ~(p_soft_pwm_h->control_pin << 16);
//            p_soft_pwm_h->p_duty_cycle_buf[step] |= p_soft_pwm_h->control_pin;
//        }
//        else
//        {
//            /* Reset pin */
//            p_soft_pwm_h->p_duty_cycle_buf[step] |=
//                (p_soft_pwm_h->control_pin << 16);
//            p_soft_pwm_h->p_duty_cycle_buf[step] &= ~p_soft_pwm_h->control_pin;
//        }
//    }
}

void tickSoftPWM(TIM_HandleTypeDef *htim) {}
