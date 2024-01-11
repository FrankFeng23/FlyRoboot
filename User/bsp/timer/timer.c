#include "timer.h"
#include "../led/bsp_led.h"


TIM_HandleTypeDef htimer2;
TIM_HandleTypeDef htimer3;

//定时器2初始化，输出PWM波控制电机,周期500ms
void timer2_config(void)
{
    TIM_OC_InitTypeDef TIM_OC_InitStructure;

    htimer2.Instance = TIM2;
    htimer2.Init.Prescaler = 7199;
    htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htimer2.Init.Period = 499;
    htimer2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&htimer2);
    HAL_TIM_PWM_Init(&htimer2);
    

    TIM_OC_InitStructure.OCMode = TIM_OCMODE_PWM1;
    TIM_OC_InitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
    TIM_OC_InitStructure.OCFastMode = TIM_OCFAST_DISABLE;
    TIM_OC_InitStructure.Pulse = 250;
    

    HAL_TIM_PWM_ConfigChannel(&htimer2,&TIM_OC_InitStructure,TIM_CHANNEL_2);
    

    HAL_TIM_Base_Start(&htimer2);
    HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_2);


}

//定时器3初始化，3s定时循环
void timer3_config(void)
{
    htimer3.Instance = TIM3;
    htimer3.Init.Prescaler = 7199;
    htimer3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htimer3.Init.Period = 29999;
    htimer3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_TIM_Base_Init(&htimer3);
    HAL_TIM_Base_Start_IT(&htimer3);
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
{
    
    if(htim == &htimer2)
    {
        TIM2_CLK_ENABLE();
        TIM2_GPIO_CLK_ENABLE();

        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
        GPIO_InitStructure.Pin = (TIM2_CH1_PIN | TIM2_CH2_PIN | TIM2_CH3_PIN | TIM2_CH4_PIN);

        HAL_GPIO_Init(TIM2_GPIO_PORT,&GPIO_InitStructure);
    }
    

    if(htim == &htimer3)
    {
        TIM3_CLK_ENABLE();

        HAL_NVIC_SetPriority(TIM3_IRQn,4,0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }

}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if(htim==&htimer3)
    {
        HAL_GPIO_TogglePin(LED3_GPIO_PORT,LED3_PIN);
    }
}


