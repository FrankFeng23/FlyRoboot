#include "nrf24l01.h"
#include "../mpu6050/mpu6050.h"

uint8_t tx_adr_value[5]={0x01,0x02,0x03,0x04,0x05};
uint8_t rx_adr0_value[5]={0x01,0x02,0x03,0x04,0x05};
/*
1��д Tx �ڵ�ĵ�ַ
TX_ADDR
2��д Rx �ڵ�ĵ�ַ����Ҫ��Ϊ��ʹ�� Auto Ack��
RX_ADDR_P0
3��ʹ�� AUTO ACK
EN_AA
4��ʹ�� PIPE 0
EN_RXADDR
5�������Զ��ط�����
SETUP_RETR
6��ѡ��ͨ��Ƶ��
RF_CH
7�����÷������������Ŵ������桢���书�ʡ��������ʣ�
RF_SETUP
8 ) ѡ��ͨ�� 0 ��Ч���ݿ��
Rx_Pw_P0
9������ 24L01 �Ļ��������Լ��л�����ģʽ
CONFIG
*/
void nrf_tx_mode(void)
{
    NRF_CE_0;
    spi_w_buffer(NRF_WRITE_REG+TX_ADDR,tx_adr_value,5);
    spi_w_buffer(NRF_WRITE_REG+RX_ADDR_P0,rx_adr0_value,5);    
    spi_w_reg(NRF_WRITE_REG+EN_AA,0x01);   //����ͨ��0�Զ�Ӧ������
    spi_w_reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //��������ͨ��0����
    spi_w_reg(NRF_WRITE_REG+SETUP_RETR,0x03);
    spi_w_reg(NRF_WRITE_REG+RF_CH,0x02);
    spi_w_reg(NRF_WRITE_REG+RF_SETUP,0x09);
    spi_w_reg(NRF_WRITE_REG+RX_PW_P0,0x01);
    spi_w_reg(NRF_WRITE_REG+CONFIG,0x01);
    NRF_CE_1;
}
/*
1��д Rx �ڵ�ĵ�ַ
RX_ADDR_P0
2��ʹ�� AUTO ACK
EN_AA
3��ʹ�� PIPE 0
EN_RXADDR
4��ѡ��ͨ��Ƶ��
RF_CH
5) ѡ��ͨ�� 0 ��Ч���ݿ��
Rx_Pw_P0
6�����÷������������Ŵ������桢���书�ʡ��������ʣ�
RF_SETUP
7������ 24L01 �Ļ��������Լ��л�����ģʽ
CONFIG
*/
void nrf_rx_mode(void)
{
    NRF_CE_0;
    spi_w_buffer(NRF_WRITE_REG+RX_ADDR_P0,rx_adr0_value,5);    
    spi_w_reg(NRF_WRITE_REG+EN_AA,0x01);   //����ͨ��0�Զ�Ӧ������
    spi_w_reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //��������ͨ��0����
    spi_w_reg(NRF_WRITE_REG+RF_CH,0x02);
    spi_w_reg(NRF_WRITE_REG+RF_SETUP,0x09);
    spi_w_reg(NRF_WRITE_REG+RX_PW_P0,0x01);
    spi_w_reg(NRF_WRITE_REG+CONFIG,0x01);
    NRF_CE_1;
}


//nrf�������ݰ�
uint8_t nrf_tx_packet(uint8_t* txbuffer)
{
    uint8_t sta;
    NRF_CE_0;
    //д���ݵ����ͻ�����
    spi_w_buffer(WR_TX_PLOAD,txbuffer,0x32);
    NRF_CE_1;  
    //�жϷ�������ж��ź�
    while(NRF_IRQ_READ());
    //��ȡ״̬�Ĵ���ֵ
    sta=spi_r_reg(STATUS,sta);
    //д״̬�Ĵ���������жϱ�־
    spi_w_reg(NRF_WRITE_REG+STATUS,sta);
    if (sta&MAX_TX)
    {
        //��շ��ͻ�����
        spi_w_reg(FLUSH_TX,0xff);
        return MAX_TX;
    }
    else if (sta&TX_OK)
    {
        return 1;
    }
    return 0;
}

//NRF�������ݰ�
uint8_t nrf_rx_packet(uint8_t* rxbuffer)
{
    uint8_t sta;
    NRF_CE_1;
    //��ȡSTATUS״̬�Ĵ�����ֵ
    sta = spi_r_reg(STATUS);
    //д״̬�Ĵ���������жϱ�־
    spi_w_reg(NRF_WRITE_REG+STATUS,sta);
    if(sta&RX_OK)
    {
        //�ӽ��ջ�������ȡ����
        spi_r_buffer(RD_RX_PLOAD,rxbuffer,0x32);
        //������ݻ�����
        spi_w_reg(FLUSH_RX,0xff);
        return 1;
    }
    return 0;
}

