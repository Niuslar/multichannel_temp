/**
 * @file uart_com.h
 * @brief This file contains the defines and function prototypes used with UART
 */

/*
 *  Created on: Jan 20, 2022
 *      Author: niuslar
 */

#ifndef INC_UART_COM_H_
#define INC_UART_COM_H_

#include "main.h"

#define UART_ENABLE		0
#define UART_DISABLE 	1
#define UART_TIMEOUT	40
#define SEND_ADC		0
#define SEND_CELSIUS	1


/* Exported Function Prototypes */
void uartSendData(UART_HandleTypeDef *huart, uint8_t raw_or_celsius);

#endif /* INC_UART_COM_H_ */