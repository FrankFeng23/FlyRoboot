#ifndef __UART_H_
#define __UART_H_

#include "stm32f1xx.h"
#include <stdio.h>

#define LENGTHMAX                           10

#define UART_TX_GPIO_PORT                   GPIOA
#define UART_TX_PIN                         GPIO_PIN_9

#define UART_RX_GPIO_PORT                   GPIOA
#define UART_RX_PIN                         GPIO_PIN_10

#define UART_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART_CLK_ENABLE()                   __HAL_RCC_USART1_CLK_ENABLE()

extern UART_HandleTypeDef huart1;
extern uint8_t buffer[LENGTHMAX];
extern uint8_t receive_state;

void User_Uart_config(void);
void HAL_UART_MspInit(UART_HandleTypeDef* huart);
















#endif

