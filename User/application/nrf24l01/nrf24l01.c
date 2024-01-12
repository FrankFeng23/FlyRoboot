#include "nrf24l01.h"
#include "../mpu6050/mpu6050.h"

uint8_t tx_adr_value[5]={0x01,0x02,0x03,0x04,0x05};
uint8_t rx_adr0_value[5]={0x01,0x02,0x03,0x04,0x05};
/*
1）写 Tx 节点的地址
TX_ADDR
2）写 Rx 节点的地址（主要是为了使能 Auto Ack）
RX_ADDR_P0
3）使能 AUTO ACK
EN_AA
4）使能 PIPE 0
EN_RXADDR
5）配置自动重发次数
SETUP_RETR
6）选择通信频率
RF_CH
7）配置发射参数（低噪放大器增益、发射功率、无线速率）
RF_SETUP
8 ) 选择通道 0 有效数据宽度
Rx_Pw_P0
9）配置 24L01 的基本参数以及切换工作模式
CONFIG
*/
void nrf_tx_mode(void)
{
    NRF_CE_0;
    spi_w_buffer(NRF_WRITE_REG+TX_ADDR,tx_adr_value,5);
    spi_w_buffer(NRF_WRITE_REG+RX_ADDR_P0,rx_adr0_value,5);    
    spi_w_reg(NRF_WRITE_REG+EN_AA,0x01);   //数据通道0自动应答允许
    spi_w_reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //接收数据通道0允许
    spi_w_reg(NRF_WRITE_REG+SETUP_RETR,0x03);
    spi_w_reg(NRF_WRITE_REG+RF_CH,0x02);
    spi_w_reg(NRF_WRITE_REG+RF_SETUP,0x09);
    spi_w_reg(NRF_WRITE_REG+RX_PW_P0,0x01);
    spi_w_reg(NRF_WRITE_REG+CONFIG,0x01);
    NRF_CE_1;
}
/*
1）写 Rx 节点的地址
RX_ADDR_P0
2）使能 AUTO ACK
EN_AA
3）使能 PIPE 0
EN_RXADDR
4）选择通信频率
RF_CH
5) 选择通道 0 有效数据宽度
Rx_Pw_P0
6）配置发射参数（低噪放大器增益、发射功率、无线速率）
RF_SETUP
7）配置 24L01 的基本参数以及切换工作模式
CONFIG
*/
void nrf_rx_mode(void)
{
    NRF_CE_0;
    spi_w_buffer(NRF_WRITE_REG+RX_ADDR_P0,rx_adr0_value,5);    
    spi_w_reg(NRF_WRITE_REG+EN_AA,0x01);   //数据通道0自动应答允许
    spi_w_reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //接收数据通道0允许
    spi_w_reg(NRF_WRITE_REG+RF_CH,0x02);
    spi_w_reg(NRF_WRITE_REG+RF_SETUP,0x09);
    spi_w_reg(NRF_WRITE_REG+RX_PW_P0,0x01);
    spi_w_reg(NRF_WRITE_REG+CONFIG,0x01);
    NRF_CE_1;
}


//nrf发送数据包
uint8_t nrf_tx_packet(uint8_t* txbuffer)
{
    uint8_t sta;
    NRF_CE_0;
    //写数据到发送缓冲区
    spi_w_buffer(WR_TX_PLOAD,txbuffer,0x32);
    NRF_CE_1;  
    //判断发送完成中断信号
    while(NRF_IRQ_READ());
    //读取状态寄存器值
    sta=spi_r_reg(STATUS,sta);
    //写状态寄存器，清空中断标志
    spi_w_reg(NRF_WRITE_REG+STATUS,sta);
    if (sta&MAX_TX)
    {
        //清空发送缓冲区
        spi_w_reg(FLUSH_TX,0xff);
        return MAX_TX;
    }
    else if (sta&TX_OK)
    {
        return 1;
    }
    return 0;
}

//NRF接收数据包
uint8_t nrf_rx_packet(uint8_t* rxbuffer)
{
    uint8_t sta;
    NRF_CE_1;
    //获取STATUS状态寄存器的值
    sta = spi_r_reg(STATUS);
    //写状态寄存器，清除中断标志
    spi_w_reg(NRF_WRITE_REG+STATUS,sta);
    if(sta&RX_OK)
    {
        //从接收缓冲区读取数据
        spi_r_buffer(RD_RX_PLOAD,rxbuffer,0x32);
        //清空数据缓冲区
        spi_w_reg(FLUSH_RX,0xff);
        return 1;
    }
    return 0;
}

