/**
 * @file uart_com.h
 * @brief This file contains the defines and function prototypes used with UART
 */

/*
 *  Created on: Jan 20, 2022
 *      Author: niuslar
 */

#ifndef UART_COM_H_
#define UART_COM_H_

#include "adc_data.h"
#include "main.h"

#define UART_ENABLE    0
#define UART_DISABLE   1
#define UART_TIMEOUT   40
#define SEND_ADC       0
#define SEND_CELSIUS   1
#define SEND_TELEMETRY 2
/* Max message length is "temp(+ADC_CHANNELS*7+);+NULL" - 1 */
#define TX_MSG_LEN          (ADC_CHANNELS * 7 + 8)
#define TEMPORAL_STRING_LEN 10

/* Exported Function Prototypes */
void uartSendData(UART_HandleTypeDef *huart, uint8_t raw_or_celsius);

#endif /* UART_COM_H_ */
