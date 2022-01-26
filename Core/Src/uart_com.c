/**
 * @file uart_com.c
 * @brief Collection of useful functions that use UART to send data
 */

/*
 *  Created on: Jan 20, 2022
 *      Author: niuslar
 */

#include "main.h"
#include "uart_com.h"
#include "adc_temp.h"
#include "stdio.h"

/* Private variables */
static uint32_t time_stamp = 0;
static uint32_t send_delay = 50; /* Min time delay between calls in ms*/
static char uart_tx_buf[ADC_CHANNELS][TX_MSG_LEN];

/**
  * @brief Sends selected data via blocking UART
  * @note This function will send data at a min of "send_delay" intervals in ms
  * @param huart handle
  * @param raw_or_celsius can be either SEND_ADC or SEND_CELSIUS
  * @retval None
  */
void uartSendData(UART_HandleTypeDef *huart, uint8_t adc_or_celsius)
{
	/* Send data after selected time delay */
	if((HAL_GetTick() - time_stamp) > send_delay)
	{
		/* Enable UART Pin*/
		HAL_GPIO_WritePin(USART_DE_GPIO_Port, USART_DE_Pin, UART_ENABLE);

		if(adc_or_celsius == SEND_ADC)
		{
			const uint16_t* tmp_p = readADCData();
			for(uint8_t channel = 0; channel < ADC_CHANNELS; channel++)
			{
				sprintf(uart_tx_buf[channel], "ADC CH.%d is: %d\n\r", channel+1, tmp_p[channel]);
			}

			if(HAL_TIMEOUT == HAL_UART_Transmit(huart, (uint8_t*)uart_tx_buf, ADC_CHANNELS*TX_MSG_LEN, UART_TIMEOUT))
			{
				Error_Handler();
			}
		}
		else if(adc_or_celsius == SEND_CELSIUS)
		{
			const float* tmp_p = readTempSensors();
			for(uint8_t channel = 0; channel < ADC_CHANNELS; channel++)
			{
				sprintf(uart_tx_buf[channel], "Temp CH.%d is: %.2f\n\r", channel+1, tmp_p[channel]);
			}

			if(HAL_OK != HAL_UART_Transmit(huart, (uint8_t*)uart_tx_buf, ADC_CHANNELS*TX_MSG_LEN, UART_TIMEOUT))
			{
				Error_Handler();
			}
		}
		else
		{
			Error_Handler();
		}

		time_stamp = HAL_GetTick();

		/* Disable UART Pin*/
		HAL_GPIO_WritePin(USART_DE_GPIO_Port, USART_DE_Pin, UART_DISABLE);
	}
}


