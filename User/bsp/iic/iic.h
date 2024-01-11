#ifndef __IIC_H_
#define __IIC_H_


#include "stm32f1xx.h"

#define IIC_SCL_GPIO_PORT                   GPIOB
#define IIC_SCL_BIN                         GPIO_PIN_6 

#define IIC_SDA_GPIO_PORT                   GPIOB
#define IIC_SDA_PIN                         GPIO_PIN_7

#define IIC_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()

#define IIC_SCL_H                           GPIOB->BSRR=GPIO_PIN_6 \
                                             /* HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET) */
#define IIC_SCL_L                           GPIOB->BRR=GPIO_PIN_6 
#define IIC_SDA_H                           GPIOB->BSRR=GPIO_PIN_7
#define IIC_SDA_L                           GPIOB->BRR=GPIO_PIN_7

#define IIC_SDA_READ()                      GPIOB->IDR&GPIO_PIN_7


void iic_gpio_config(void);
void iic_start(void);
void iic_stop(void);
void iic_SendByte(uint8_t Sendbyte);
uint8_t iic_ReceiveByte(void);
uint8_t iic_WaitAck(void);
void iic_Ack(void);
void iic_NAck(void);
uint8_t iic_CheckDevice(uint8_t address);

#endif

