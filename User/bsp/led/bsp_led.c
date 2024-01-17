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
st_led LED;
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
    
    LED.Flashingtime=1000;
    LED.status=AlternateFlash;
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


void LED_Control(void)
{
  static uint32_t lasttame=0;
  if(systick_count-lasttame<LED.Flashingtime)
  {
      return;
  }
  else
      lasttame=systick_count;
  switch (LED.status)
  {
  case AlwaysON:
    LED3_L;
    LED4_L;
    break;
  case AlwaysOFF:
    LED3_H;
    LED4_H;
    break;
  case Flashing:
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0|GPIO_PIN_1);
    break;
  case AlternateFlash:
    LED3_H;
    LED4_L;
    LED.status=Flashing;
    break;
  default:
    break;
  }
}

/*********************************************END OF FILE**********************/
