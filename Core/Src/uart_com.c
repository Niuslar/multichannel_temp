/**
 * @file uart_com.c
 * @brief Collection of useful functions that use UART to send data
 */

/*
 *  Created on: Jan 20, 2022
 *      Author: niuslar
 */

#include "temperature.h"
#include "main.h"
#include "uart_com.h"
#include "telemetry.h"
#include "adc_data.h"
#include "stdio.h"
#include "string.h"

/* Private variables */
static char uart_tx_buf[TX_MSG_LEN];
static char tmp_string[TMP_STRING_LEN];

/**
  * @brief Sends selected data via blocking UART
  * @param huart handle
  * @param adc_data_type can be of the type SEND_ADC | SEND_CELSIUS | SEND_TELE
  * @retval None
  */
void uartSendData(UART_HandleTypeDef *huart, uint8_t adc_data_type)
{

	/* Enable UART Pin*/
	HAL_GPIO_WritePin(USART_DE_GPIO_Port, USART_DE_Pin, UART_ENABLE);

	if(adc_data_type == SEND_ADC)
	{
		const uint16_t* tmp_p = getADCData();
		strcat(uart_tx_buf, ">adc(");
		for(uint8_t channel = 0; channel < ADC_CHANNELS; channel++)
		{
			if(channel < ADC_CHANNELS-1)
			{
				sprintf(tmp_string, "%d,", tmp_p[channel]);
				strcat(uart_tx_buf, tmp_string);
			}
			if(channel == ADC_CHANNELS-1)
			{
				sprintf(tmp_string, "%d);\n", tmp_p[channel]);
				strcat(uart_tx_buf, tmp_string);
			}
		}

		if(HAL_OK != HAL_UART_Transmit(huart, (uint8_t*)uart_tx_buf, strlen(uart_tx_buf)+1, UART_TIMEOUT))
		{
			Error_Handler();
		}
	}
	else if(adc_data_type == SEND_CELSIUS)
	{
		const float* tmp_p = readTemp();
		strcat(uart_tx_buf, ">temp(");
		for(uint8_t channel = 0; channel < TEMP_CHANNELS; channel++)
		{
			if(channel < TEMP_CHANNELS-1)
			{
				sprintf(tmp_string, "%.2f,", tmp_p[channel]);
				strcat(uart_tx_buf, tmp_string);
			}
			if(channel == TEMP_CHANNELS-1)
			{
				sprintf(tmp_string, "%.2f);\n", tmp_p[channel]);
				strcat(uart_tx_buf, tmp_string);
			}
		}

		if(HAL_OK != HAL_UART_Transmit(huart, (uint8_t*)uart_tx_buf, strlen(uart_tx_buf)+1, UART_TIMEOUT))
		{
			Error_Handler();
		}
	}

	else if(adc_data_type == SEND_TELE)
	{
		const float* tmp_p = readTele();
		/* Telemetry message format is: >tele(Vin, It, Ic, AT); */
		strcat(uart_tx_buf, ">tele(");
		for(uint8_t channel = 0; channel < TELE_CHANNELS ; channel++)
		{
			if(channel < TELE_CHANNELS-1)
			{
				sprintf(tmp_string, "%.2f,", tmp_p[channel]);
				strcat(uart_tx_buf, tmp_string);
			}
			if(channel == TELE_CHANNELS-1)
			{
				sprintf(tmp_string, "%.2f);\n", tmp_p[channel]);
				strcat(uart_tx_buf, tmp_string);
			}
		}

		if(HAL_OK != HAL_UART_Transmit(huart, (uint8_t*)uart_tx_buf, strlen(uart_tx_buf)+1, UART_TIMEOUT))
		{
			Error_Handler();
		}
	}

	else
	{
		Error_Handler();
	}

	/* Reset buffer */
	uart_tx_buf[0] = '\0';

	/* Disable UART Pin*/
	HAL_GPIO_WritePin(USART_DE_GPIO_Port, USART_DE_Pin, UART_DISABLE);

}


