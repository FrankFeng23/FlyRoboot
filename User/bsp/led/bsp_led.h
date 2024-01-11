#ifndef __LED_H
#define	__LED_H

#include "stm32f1xx.h"

#define LED1_GPIO_PORT					GPIOA
#define LED1_PIN						GPIO_PIN_7

#define LED2_GPIO_PORT					GPIOA
#define LED2_PIN						GPIO_PIN_6

#define LED3_GPIO_PORT					GPIOB
#define LED3_PIN						GPIO_PIN_1

#define LED4_GPIO_PORT					GPIOB
#define LED4_PIN						GPIO_PIN_0

#define LED_GPIO_CLK_ENABLE()				do {  \
										__HAL_RCC_GPIOA_CLK_ENABLE(); \
										__HAL_RCC_GPIOB_CLK_ENABLE(); \
										}while(0)

#define LED1_H							GPIOA->BSRR=LED1_PIN
#define LED1_L							GPIOA->BRR=LED1_PIN

#define LED2_H							GPIOA->BSRR=LED2_PIN
#define LED2_L							GPIOA->BRR=LED2_PIN

#define LED3_H							GPIOB->BSRR=LED3_PIN
#define LED3_L							GPIOB->BRR=LED3_PIN

#define LED4_H							GPIOB->BSRR=LED4_PIN
#define LED4_L							GPIOB->BRR=LED4_PIN

void LED_GPIO_Config(void);


#endif /* __LED_H */
