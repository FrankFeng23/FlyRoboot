#include "iic.h"

void iic_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //GPIO使能
    IIC_GPIO_CLK_ENABLE();
    //IIC_SCL时钟输出模式配置
    GPIO_InitStructure.Pin=IIC_SCL_BIN;
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT,&GPIO_InitStructure);
    //IIC_SDA数据输出模式配置
    GPIO_InitStructure.Pin=IIC_SDA_PIN;
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT,&GPIO_InitStructure);
    iic_stop();
}

static void iic_delay (void)
{
    uint8_t i;
    for(i=10;i<=0;i--);
}
//iic协�??�?动信�?
void iic_start(void)
{
    IIC_SDA_H;
    IIC_SCL_H;
    iic_delay();
    IIC_SDA_L;
    iic_delay();
    IIC_SCL_L;
    iic_delay();
}
//iic协�??停�??信号
void iic_stop(void)
{
    IIC_SDA_L;
    IIC_SCL_H;
    iic_delay();
    IIC_SDA_H;
    iic_delay();
}
//iic发送单字节
void iic_SendByte(uint8_t Sendbyte)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        if(Sendbyte & 0x80)
        {
            IIC_SDA_H;
        }
        else
        {
            IIC_SDA_L;
        }
        iic_delay();
        IIC_SCL_H;
        iic_delay();
        IIC_SCL_L;
        if(i==7)
        {
            IIC_SDA_H;   //输出高阻态，释放SDA总线
        }
        Sendbyte<<=1;
        iic_delay();
    }
}
//iic接收单字�?
uint8_t iic_ReceiveByte(void)
{
    uint8_t Result=0;
    uint8_t i;
    for(i=0;i<8;i++)
    {
        Result<<=1;
        IIC_SCL_H;
        iic_delay();
        if(IIC_SDA_READ())
        {
            Result++;
        }
        IIC_SCL_L;
        iic_delay();
    }
    return Result;
}
//等待应答信号
//相应成功返回1，否则返�?0
uint8_t iic_WaitAck(void)
{
    uint8_t value;
    IIC_SDA_H;          //输出高阻态，释放SDA总线   
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    if(IIC_SDA_READ())
    {
        value = 0;
    }
    else
    {
        value = 1;
    }
    IIC_SCL_L;
    iic_delay();
    return value;
}
//发送响应信�?
void iic_Ack(void)
{
    IIC_SDA_L;
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    IIC_SCL_L;
    iic_delay();
    IIC_SDA_H;      //释放SDA总线
    iic_delay();
}
//发送非应答信号
void iic_NAck(void)
{
    IIC_SDA_H;
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    IIC_SCL_L;
    iic_delay();
}

uint8_t iic_CheckDevice(uint8_t slave_adr)
{
    uint8_t result;
    iic_start();
    iic_SendByte(slave_adr);
    result = iic_WaitAck();
    iic_stop();
    return result;
}
