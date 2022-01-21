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

/* Private variables */
static uint32_t time_stamp = 0;
static uint32_t send_delay = 50; /* Min time delay between calls in ms*/

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
			/* ADC_CHANNELS * 2 is the size of the transfer */
			if(HAL_TIMEOUT == HAL_UART_Transmit(huart, (uint8_t*)tmp_p, ADC_CHANNELS*2, UART_TIMEOUT))
			{
				Error_Handler();
			}
		}
		else if(adc_or_celsius == SEND_CELSIUS)
		{
			const float* tmp_p = readTempSensors();
			/* Because "float" is 4 bytes, size of transfer is ADC_CHANNELS *4 */
			if(HAL_OK != HAL_UART_Transmit(huart, (uint8_t*)tmp_p, ADC_CHANNELS*4, UART_TIMEOUT))
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


