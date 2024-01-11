#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f1xx.h"

#define ADC_GPIO_PORT                           GPIOC
#define YAH_PIN                                 GPIO_PIN_0
#define THR_PIN                                 GPIO_PIN_1
#define PITCH_PIN                               GPIO_PIN_2
#define ROLL_PIN                                GPIO_PIN_3


#define ADC_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()    
#define ADC_CLK_ENABLE()                        __HAL_RCC_ADC1_CLK_ENABLE()      
#define DMA_CLK_ENABLE()                        __HAL_RCC_DMA1_CLK_ENABLE()           

void adc_config(void);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);

extern uint16_t ADC_DMA_BUFFER[4];


#endif


