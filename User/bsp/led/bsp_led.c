/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   led??Þ~??????
  ******************************************************************************
  * @attention
  *
  * ?????:??? STM32 F103 ??????  
  * ???    :http://www.firebbs.cn
  * ???    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_led.h"   

 /**
  * @brief  ?????????LED??IO
  * @param  ??
  * @retval ??
  */
void LED_GPIO_Config(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;

    LED_GPIO_CLK_ENABLE();

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = (LED1_PIN | LED2_PIN);
    HAL_GPIO_Init(LED1_GPIO_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.Pin = (LED3_PIN | LED4_PIN);
    HAL_GPIO_Init(LED3_GPIO_PORT,&GPIO_InitStructure);
	
		LED1_H;
		LED2_H;
		LED3_H;
		LED4_H;
  
}



void Led_BlingBling(void)
{
  LED1_L;
  LED2_L;
  LED3_L;
  LED4_L;
  HAL_Delay(500);
  LED1_H;
  LED2_H;
  LED3_H;
  LED4_H;
  HAL_Delay(500);
  
}



/*********************************************END OF FILE**********************/
