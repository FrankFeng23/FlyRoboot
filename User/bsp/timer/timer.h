#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f1xx.h"




#define TIM2_GPIO_PORT                        GPIOA
#define TIM2_CH1_PIN                          GPIO_PIN_0
#define TIM2_CH2_PIN                          GPIO_PIN_1
#define TIM2_CH3_PIN                          GPIO_PIN_2
#define TIM2_CH4_PIN                          GPIO_PIN_3

#define TIM2_CLK_ENABLE()                     __HAL_RCC_TIM2_CLK_ENABLE()
#define TIM3_CLK_ENABLE()                     __HAL_RCC_TIM3_CLK_ENABLE()
#define TIM2_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE()

extern volatile uint32_t systick_count;
extern TIM_HandleTypeDef htimer2;
extern TIM_HandleTypeDef htimer3;



void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim);
void timer2_config(void);
void timer3_config(void);





#endif


