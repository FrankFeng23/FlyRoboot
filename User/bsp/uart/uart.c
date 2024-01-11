#include "uart.h"


UART_HandleTypeDef huart1;
uint8_t buffer[LENGTHMAX] = {0};
uint8_t receive_state = 0;

//串口初始化函数
void User_Uart_config(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    
    HAL_UART_Init(&huart1);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buffer,LENGTHMAX);

}


//串口1GPIO初始化函数
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance == USART1)
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        UART_GPIO_CLK_ENABLE();
        UART_CLK_ENABLE();

        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pin = UART_TX_PIN;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(UART_TX_GPIO_PORT,&GPIO_InitStructure);

        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Pin = UART_RX_PIN;
        HAL_GPIO_Init(UART_RX_GPIO_PORT,&GPIO_InitStructure);

        HAL_NVIC_SetPriority(USART1_IRQn,3,0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
}

//???¨?òc??????printf??????DEBUG_USART?????¨?ò?ó??????printf????
int fputc(int ch, FILE *f)
{
	/* ·???????×?????????????DEBUG_USART */
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

//???¨?òc??????scanf??????DEBUG_USART???????ò?ó??????scanf??getchar??????
int fgetc(FILE *f)
{		
	int ch;
	HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart == &huart1)
    {
        HAL_UART_Transmit(&huart1,buffer,Size,100);

        HAL_UARTEx_ReceiveToIdle_IT(&huart1,buffer,LENGTHMAX);
    }
    
}



