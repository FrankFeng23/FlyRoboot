#include "at24c02.h"
//EE写数据
void iic_ee_write_bytes(uint8_t slave_adr,
                        uint8_t reg_adr,
                        uint8_t* pbuffer,
                        uint8_t len)
{
    uint8_t i,m;
    uint8_t address;
    address=reg_adr;
    for(i=0;i<len;i++)
    {
        if(i==0 || (address&(AT24C02_PAGE_SIZE-1))==0)
        {
            iic_stop();
            for(m=0;m<1000;m++)
            {
                iic_start();
                iic_SendByte(slave_adr);
                if(iic_WaitAck())
                {
                    break;
                }
            }
            iic_SendByte(address);
            while(iic_WaitAck()==0);
        }
        iic_SendByte(pbuffer[i]);
        while(iic_WaitAck()==0);
		address++;
    }
    iic_stop();
}
//EE读数据
void iic_ee_read_bytes(uint8_t slave_adr,
                       uint8_t reg_adr,
                       uint8_t* pbuffer,
                       uint8_t len)
{
    uint8_t i;
    iic_start();
    iic_SendByte(slave_adr);
    while(iic_WaitAck()==0);
    iic_SendByte(reg_adr);
    while(iic_WaitAck()==0);
    iic_start();
    iic_SendByte(slave_adr+1);
    while(iic_WaitAck()==0);
    for(i=0;i<len;i++)
    {
        pbuffer[i]=iic_ReceiveByte();
        (i==len-1) ? iic_NAck() : iic_Ack();
    }
    iic_stop();
}

