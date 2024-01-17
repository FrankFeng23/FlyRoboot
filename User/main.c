/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ????led
  ******************************************************************************
  * @attention
  *
  * ?????:??? F103-??? STM32 ?????? 
  * ???    :http://www.firebbs.cn
  * ???    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx.h"
#include "./bsp/led/bsp_led.h"
#include "./bsp/iic/iic.h"
#include "./bsp/timer/timer.h"
#include "./bsp/uart/uart.h"
#include "./bsp/adc/adc.h"
#include "./application/mpu6050/mpu6050.h"
#include "./application/at24c02/at24c02.h"
#include "./math/imu/imu.h"


/**
  * @brief  ??????
  * @param  ??
  * @retval ??
  */
int main(void)
{
//  uint8_t write_buffer[15]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
//  uint8_t read_buffer[15]={0};
    SystemClock_Config();
    LED_GPIO_Config();
    iic_gpio_config();
    timer2_config();
	  timer3_config();
    User_Uart_config();
    adc_config();
    MPU6050_Init();
    while (1)
    {
      
      LED_Control();
      
      
      /* 
      if(MPU6050_ID_Text() != 0x68)
      {
        printf("mpu6050检测出错");
        return 0;
      }
      mpu6050_read_accel_data(acc_buffer);
      mpu6050_read_gyro_data(gyro_buffer);
      printf("acc_x = %d,acc_y = %d,acc_z = %d\n",acc_buffer[0],acc_buffer[1],acc_buffer[2]);
      printf("gyro_x = %d,gyro_y = %d,gyro_z = %d\n",gyro_buffer[0],gyro_buffer[1],gyro_buffer[2]);
      HAL_Delay(2000);
    
		
    printf("\r\n CH0 value = %d V \r\n",ADC_DMA_BUFFER[0]);
		printf("\r\n CH1 value = %d V \r\n",ADC_DMA_BUFFER[1]);
		printf("\r\n CH2 value = %d V \r\n",ADC_DMA_BUFFER[2]);
		printf("\r\n CH3 value = %d V \r\n",ADC_DMA_BUFFER[3]);
    */

   /*if(iic_CheckDevice(AT24C02_ADR))
    {
      uint8_t i;
      printf("这是一个AT24C02测试实验\n");
      iic_ee_write_bytes(AT24C02_ADR,1,write_buffer,15);
      HAL_Delay(100);
      iic_ee_read_bytes(AT24C02_ADR,1,read_buffer,15);
      for(i=0;i<15;i++)
      {
        printf("%d ",read_buffer[i]);
      }
    }
    else
    {
      printf("AT24C02检测失败");

    }*/
    
    }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 2
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 0
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    while(1);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
