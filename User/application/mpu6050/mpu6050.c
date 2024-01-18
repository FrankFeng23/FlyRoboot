#include "mpu6050.h"

//mpu6050原始数据

st_mpu mpu6050_data;
//写单字节数据
static void iic_mpu6050_write_one_byte(uint8_t slave_adr,
                                       uint8_t reg_adr,
                                       uint8_t data)
{
    //发送起始信号
    iic_start();
    //发送读写设备地址+读写信号
    iic_SendByte(slave_adr);
    //等待从站相应
    while(iic_WaitAck()==0);
    //发送读写寄存器地址
    iic_SendByte(reg_adr);
    //等待从站相应
    while(iic_WaitAck()==0);
    //发送数据
    iic_SendByte(data);
    //等待从站相应
    while(iic_WaitAck()==0);
    //发送停止信号
    iic_stop();
}

//写多字节数据
static void iic_mpu6050_write_bytes(uint8_t slave_adr,
                                    uint8_t reg_adr,
                                    uint8_t* buffer,
                                    uint8_t len)
{
    uint8_t i;
    //发送起始信号
    iic_start();
    //发送读写设备地址+读写信号
    iic_SendByte(slave_adr);
    //等待从站相应
    while(iic_WaitAck()==0);
    //发送读写寄存器地址
    iic_SendByte(reg_adr);
    //等待从站相应
    while(iic_WaitAck()==0);
    //循环发送字节
    for(i=0;i<len;i++)
    {
        iic_SendByte(buffer[i]);
        while(iic_WaitAck()==0);
    }
    iic_stop();
}

//读单字节数据
static uint8_t iic_mpu6050_read_one_byte(uint8_t slave_adr,
                                         uint8_t reg_adr)
{
    uint8_t result;
    //发送起始信号
    iic_start();
    //发送读写设备地址+读写信号
    iic_SendByte(slave_adr);
    //等待从站相应
    while(iic_WaitAck()==0);
    //发送读写寄存器地址
    iic_SendByte(reg_adr);
    //等待从站相应
    while(iic_WaitAck()==0);
    //发送起始信号
    iic_start();
    //发送读写设备地址+读写信号
    iic_SendByte(slave_adr+1);
    //等待从站相应
    while(iic_WaitAck()==0);
    //读取从站数据
    result=iic_ReceiveByte();
    //发送非相应信号
    iic_NAck();
    //发送停止信号
    iic_stop();
    return result;
}

//读多字节数据
static void iic_mpu6050_read_bytes(uint8_t slave_adr,
                                   uint8_t reg_adr,
                                   uint8_t* buffer,
                                   uint8_t len)
{
    uint8_t i;
    //发送起始信号
    iic_start();
    //发送读写设备地址+读写信号
    iic_SendByte(slave_adr);
    //等待从站相应
    while(iic_WaitAck()==0);
    //发送读写寄存器地址
    iic_SendByte(reg_adr);
    //等待从站相应
    while(iic_WaitAck()==0);
    //发送起始信号
    iic_start();
    //发送读写设备地址+读写信号
    iic_SendByte(slave_adr+1);
    //等待从站相应
    while(iic_WaitAck()==0);
    //连续读出数据
    for(i=0;i<len;i++)
    {
        buffer[i]=iic_ReceiveByte();
        //判断是否是最后一个字节
        (i==len-1) ? iic_NAck() : iic_Ack();
    }
    iic_stop();

}

//MPU6050通讯检测
uint8_t MPU6050_ID_Text(void)
{
    return iic_mpu6050_read_one_byte(MPU6050_ADR,MPU6050_RA_WHO_AM_I);
}


//mpu6050初始化
void MPU6050_Init(void)
{
    HAL_Delay(100);
    //解除休眠狀態
    iic_mpu6050_write_one_byte(MPU6050_ADR,MPU6050_RA_PWR_MGMT_1,0x00);
    //陀螺儀采樣頻率
    iic_mpu6050_write_one_byte(MPU6050_ADR,MPU6050_RA_SMPLRT_DIV,0x07);
    iic_mpu6050_write_one_byte(MPU6050_ADR,MPU6050_RA_CONFIG,0x06);
    //配置加速度传感器工作在16G模式
    iic_mpu6050_write_one_byte(MPU6050_ADR,MPU6050_RA_ACCEL_CONFIG,0x18);
    //陀螺仪自检测及测量范围，经典值0x18
    iic_mpu6050_write_one_byte(MPU6050_ADR,MPU6050_RA_GYRO_CONFIG,0x18);
}

/*
//读取MPU6050加速度计数据
void mpu6050_read_accel_data(uint16_t* buffer)
{
    uint8_t buf[6] = {0};
    iic_mpu6050_read_bytes(MPU6050_ADR,MPU6050_ACC_ADR,buf,6);
    buffer[0] = buf[0]<<8 | buf[1] ;
    buffer[1] = buf[2]<<8 | buf[3] ;
    buffer[2] = buf[4]<<8 | buf[5] ;
}

//读取MPU6050陀螺仪数据
void mpu6050_read_gyro_data(uint16_t* buffer)
{
    uint8_t buf[6] = {0};
    iic_mpu6050_read_bytes(MPU6050_ADR,MPU6050_GYRO_ADR,buf,6);
    buffer[0] = buf[0]<<8 | buf[1] ;
    buffer[1] = buf[2]<<8 | buf[3] ;
    buffer[2] = buf[4]<<8 | buf[5] ;
}
*/


//获取MPU6050滤波后数据
void mpu6050_getdate(uint16_t* mpudate)
{
    uint8_t i;
    uint8_t buffer[12];
    iic_mpu6050_read_bytes(MPU6050_ADR,MPU6050_ACC_ADR,buffer,6);
    iic_mpu6050_read_bytes(MPU6050_ADR,MPU6050_GYRO_ADR,&buffer[6],6);
    for(i=0;i<6;i++)
    {
        mpudate[i]=(((uint16_t)buffer[i<<1]<<8) + buffer[(i<<1)+1]);
        //加速度进行一阶卡尔曼滤波
        if(i<3)
        {
            struct _1_ekf_filter ekf[3]={{0.02,0,8192,0,0.001,0.543},{0.02,0,8192,0,0.001,0.543},{0.02,0,8192,0,0.001,0.543}};
            kalman_1(&ekf[i],(float)mpudate[i]);
            mpudate[i]=(uint16_t)ekf[i].out;
        }
        //角速度进行一阶低通滤波
        else
        {
            uint8_t k=i-3;
            const float factor =0.25f;
            static float tbuf[3];
            mpudate[i]=tbuf[k]=(1-factor)*tbuf[k]+factor*mpudate[i];
        }
    }
}


