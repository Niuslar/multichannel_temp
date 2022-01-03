/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ALARM_Pin GPIO_PIN_13
#define ALARM_GPIO_Port GPIOC
#define BREATHING_Pin GPIO_PIN_14
#define BREATHING_GPIO_Port GPIOC
#define USART_DE_Pin GPIO_PIN_15
#define USART_DE_GPIO_Port GPIOC
#define ADC_1_Pin GPIO_PIN_0
#define ADC_1_GPIO_Port GPIOA
#define ADC_2_Pin GPIO_PIN_1
#define ADC_2_GPIO_Port GPIOA
#define CONTROL_3_Pin GPIO_PIN_2
#define CONTROL_3_GPIO_Port GPIOA
#define CONTROL_4_Pin GPIO_PIN_3
#define CONTROL_4_GPIO_Port GPIOA
#define ADC_3_Pin GPIO_PIN_4
#define ADC_3_GPIO_Port GPIOA
#define ADC_4_Pin GPIO_PIN_5
#define ADC_4_GPIO_Port GPIOA
#define ADC_5_Pin GPIO_PIN_6
#define ADC_5_GPIO_Port GPIOA
#define ADC_6_Pin GPIO_PIN_7
#define ADC_6_GPIO_Port GPIOA
#define ADC_7_Pin GPIO_PIN_0
#define ADC_7_GPIO_Port GPIOB
#define ADC_8_Pin GPIO_PIN_1
#define ADC_8_GPIO_Port GPIOB
#define HEAT_COOL_4_Pin GPIO_PIN_2
#define HEAT_COOL_4_GPIO_Port GPIOB
#define HEAT_COOL_3_Pin GPIO_PIN_10
#define HEAT_COOL_3_GPIO_Port GPIOB
#define TEMP_ALT_Pin GPIO_PIN_11
#define TEMP_ALT_GPIO_Port GPIOB
#define HEAT_COOL_5_Pin GPIO_PIN_12
#define HEAT_COOL_5_GPIO_Port GPIOB
#define CONTROL_5_Pin GPIO_PIN_13
#define CONTROL_5_GPIO_Port GPIOB
#define CONTROL_6_Pin GPIO_PIN_14
#define CONTROL_6_GPIO_Port GPIOB
#define HEAT_COOL_6_Pin GPIO_PIN_15
#define HEAT_COOL_6_GPIO_Port GPIOB
#define PWR_EN_Pin GPIO_PIN_8
#define PWR_EN_GPIO_Port GPIOA
#define HEAT_COOL_7_Pin GPIO_PIN_11
#define HEAT_COOL_7_GPIO_Port GPIOA
#define HEAT_COOL_8_Pin GPIO_PIN_12
#define HEAT_COOL_8_GPIO_Port GPIOA
#define CONTROL_1_Pin GPIO_PIN_15
#define CONTROL_1_GPIO_Port GPIOA
#define CONTROL_2_Pin GPIO_PIN_3
#define CONTROL_2_GPIO_Port GPIOB
#define CONTROL_7_Pin GPIO_PIN_4
#define CONTROL_7_GPIO_Port GPIOB
#define CONTROL_8_Pin GPIO_PIN_5
#define CONTROL_8_GPIO_Port GPIOB
#define HEAT_COOL_1_Pin GPIO_PIN_8
#define HEAT_COOL_1_GPIO_Port GPIOB
#define HEAT_COOL_2_Pin GPIO_PIN_9
#define HEAT_COOL_2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
