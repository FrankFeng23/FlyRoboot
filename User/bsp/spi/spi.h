#ifndef __SPI_H_
#define __SPI_H_
#include "stm32f1xx.h"

#define SPI_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_GPIO_PORT                       GPIOB

#define SPI_MOSI_PIN                        GPIO_PIN_15
#define SPI_MISO_PIN                        GPIO_PIN_14
#define SPI_SCK_PIN                         GPIO_PIN_13
#define SPI_CSN_PIN                         GPIO_PIN_4

#define SPI_SCK_1                           SPI_GPIO_PORT->BSRR = SPI_SCK_PIN
#define SPI_SCK_0                           SPI_GPIO_PORT->BRR = SPI_SCK_PIN

#define SPI_MOSI_1                          SPI_GPIO_PORT->BSRR = SPI_MOSI_PIN
#define SPI_MOSI_0                          SPI_GPIO_PORT->BRR = SPI_MOSI_PIN

#define SPI_CSN_1                           SPI_GPIO_PORT->BSRR = SPI_CSN_PIN
#define SPI_CSN_0                           SPI_GPIO_PORT->BRR = SPI_CSN_PIN

#define SPI_MISO_READ()                     (SPI_GPIO_PORT->IDR & SPI_MISO_PIN)


uint8_t spi_wr(uint8_t t_data);
void spi_w_reg(uint8_t reg,uint8_t value);
uint8_t spi_r_reg(uint8_t reg);
void spi_w_buffer(uint8_t reg,uint8_t* buffer_adr,uint8_t len);
void spi_r_buffer(uint8_t reg,uint8_t* buffer_adr,uint8_t len);












#endif

