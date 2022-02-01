/**
 * @file uart_com.c
 * @brief Collection of useful functions that use UART to send data
 */

/*
 *  Created on: Jan 20, 2022
 *      Author: niuslar
 */

#include "uart_com.h"
#include "adc_data.h"
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "telemetry.h"
#include "temperature.h"

/* Private variables */
static char uart_tx_buf[TX_MSG_LEN];
static char string[TEMPORAL_STRING_LEN];

/**
 * @brief Sends selected data via blocking UART
 * @param huart handle
 * @param adc_data_type can be of the type SEND_ADC | SEND_CELSIUS |
 * SEND_TELEMETRY
 * @retval None
 */
void uartSendData(UART_HandleTypeDef *huart, uint8_t adc_data_type)
{
    /* Enable UART Pin*/
    HAL_GPIO_WritePin(USART_DE_GPIO_Port, USART_DE_Pin, UART_ENABLE);

    if (adc_data_type == SEND_ADC)
    {
        const uint16_t *p_adc_data = getADCData();
        strcat(uart_tx_buf, ">adc(");
        for (uint8_t channel = 0; channel < ADC_CHANNELS; channel++)
        {
            if (channel < ADC_CHANNELS - 1)
            {
                sprintf(string, "%d,", p_adc_data[channel]);
                strcat(uart_tx_buf, string);
            }
            if (channel == ADC_CHANNELS - 1)
            {
                sprintf(string, "%d);\n", p_adc_data[channel]);
                strcat(uart_tx_buf, string);
            }
        }

        if (HAL_OK != HAL_UART_Transmit(huart,
                                        (uint8_t *)uart_tx_buf,
                                        strlen(uart_tx_buf) + 1,
                                        UART_TIMEOUT))
        {
            Error_Handler();
        }
    }
    else if (adc_data_type == SEND_CELSIUS)
    {
        strcat(uart_tx_buf, ">temp(");
        for (uint8_t channel = 1; channel <= TEMP_CHANNELS; channel++)
        {
            const float temperature = readTemperature(channel);
            if (channel < TEMP_CHANNELS)
            {
                sprintf(string, "%.2f,", temperature);
                strcat(uart_tx_buf, string);
            }
            if (channel == TEMP_CHANNELS)
            {
                sprintf(string, "%.2f);\n", temperature);
                strcat(uart_tx_buf, string);
            }
        }

        if (HAL_OK != HAL_UART_Transmit(huart,
                                        (uint8_t *)uart_tx_buf,
                                        strlen(uart_tx_buf) + 1,
                                        UART_TIMEOUT))
        {
            Error_Handler();
        }
    }

    else if (adc_data_type == SEND_TELEMETRY)
    {
        /* Telemetry message format is: >tele(Vin, It, Ic, AT); */
        strcat(uart_tx_buf, ">tele(");
        for (uint8_t channel = TEMP_CHANNELS + 1; channel <= ADC_CHANNELS;
             channel++)
        {
            const float telemetry = readTelemetry(channel);
            if (channel < ADC_CHANNELS)
            {
                sprintf(string, "%.2f,", telemetry);
                strcat(uart_tx_buf, string);
            }
            if (channel == ADC_CHANNELS)
            {
                sprintf(string, "%.2f);\n", telemetry);
                strcat(uart_tx_buf, string);
            }
        }

        if (HAL_OK != HAL_UART_Transmit(huart,
                                        (uint8_t *)uart_tx_buf,
                                        strlen(uart_tx_buf) + 1,
                                        UART_TIMEOUT))
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
