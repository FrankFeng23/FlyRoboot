#include "spi.h"


//SPI≥ı ºªØGPIO≈‰÷√
void spi_gpio_congig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    SPI_GPIO_CLK_ENABLE();

    GPIO_InitStructure.Pin = SPI_MOSI_PIN | SPI_SCK_PIN | SPI_CSN_PIN ;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SPI_GPIO_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.Pin = SPI_MISO_PIN ;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SPI_GPIO_PORT,&GPIO_InitStructure);

    SPI_SCK_0;

}

//SPI∑¢ÀÕ°¢Ω” ’1byte
uint8_t spi_wr(uint8_t t_data)
{
    SPI_SCK_0;
    uint8_t result;
    for(uint8_t i=0;i<8;i++)
    {
        if(t_data&0x80)
        {
            SPI_MOSI_1;
        }
        else
        {
            SPI_MOSI_0;
        }
        t_data<<=1;
        result<<=1;
        SPI_SCK_1;
        if(SPI_MISO_READ())
        {
            result++;
        }
        SPI_SCK_0;
    }
    return result;
}

//spi–¥ºƒ¥Ê∆˜
void spi_w_reg(uint8_t reg,uint8_t value)
{
    SPI_CSN_0;
    spi_wr(reg);
    spi_wr(value);
    SPI_CSN_1;
}
//spi∂¡ºƒ¥Ê∆˜
uint8_t spi_r_reg(uint8_t reg)
{
    uint8_t result;
    SPI_CSN_0;
    spi_wr(reg);
    result=spi_wr(0XFF);
    SPI_CSN_1;
    return result;
}
//spi–¥ª∫¥Ê«¯
void spi_w_buffer(uint8_t reg,uint8_t* buffer_adr,uint8_t len)
{
    uint8_t i;
    SPI_CSN_0;
    spi_wr(reg);
    for(i=0;i<len;i++)
    spi_wr(buffer_adr[i]);
    SPI_CSN_1;
}
//spi∂¡ª∫¥Ê«¯
void spi_r_buffer(uint8_t reg,uint8_t* buffer_adr,uint8_t len)
{
    uint8_t i;
    SPI_CSN_0;
    spi_wr(reg);
    for(i=0;i<len;i++)
    buffer_adr[i]=spi_wr(0xFF);
    SPI_CSN_1;
}


