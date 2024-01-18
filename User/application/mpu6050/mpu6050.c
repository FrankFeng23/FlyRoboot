#include "mpu6050.h"

//mpu6050ԭʼ����

st_mpu mpu6050_data;
//д���ֽ�����
static void iic_mpu6050_write_one_byte(uint8_t slave_adr,
                                       uint8_t reg_adr,
                                       uint8_t data)
{
    //������ʼ�ź�
    iic_start();
    //���Ͷ�д�豸��ַ+��д�ź�
    iic_SendByte(slave_adr);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //���Ͷ�д�Ĵ�����ַ
    iic_SendByte(reg_adr);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //��������
    iic_SendByte(data);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //����ֹͣ�ź�
    iic_stop();
}

//д���ֽ�����
static void iic_mpu6050_write_bytes(uint8_t slave_adr,
                                    uint8_t reg_adr,
                                    uint8_t* buffer,
                                    uint8_t len)
{
    uint8_t i;
    //������ʼ�ź�
    iic_start();
    //���Ͷ�д�豸��ַ+��д�ź�
    iic_SendByte(slave_adr);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //���Ͷ�д�Ĵ�����ַ
    iic_SendByte(reg_adr);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //ѭ�������ֽ�
    for(i=0;i<len;i++)
    {
        iic_SendByte(buffer[i]);
        while(iic_WaitAck()==0);
    }
    iic_stop();
}

//�����ֽ�����
static uint8_t iic_mpu6050_read_one_byte(uint8_t slave_adr,
                                         uint8_t reg_adr)
{
    uint8_t result;
    //������ʼ�ź�
    iic_start();
    //���Ͷ�д�豸��ַ+��д�ź�
    iic_SendByte(slave_adr);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //���Ͷ�д�Ĵ�����ַ
    iic_SendByte(reg_adr);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //������ʼ�ź�
    iic_start();
    //���Ͷ�д�豸��ַ+��д�ź�
    iic_SendByte(slave_adr+1);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //��ȡ��վ����
    result=iic_ReceiveByte();
    //���ͷ���Ӧ�ź�
    iic_NAck();
    //����ֹͣ�ź�
    iic_stop();
    return result;
}

//�����ֽ�����
static void iic_mpu6050_read_bytes(uint8_t slave_adr,
                                   uint8_t reg_adr,
                                   uint8_t* buffer,
                                   uint8_t len)
{
    uint8_t i;
    //������ʼ�ź�
    iic_start();
    //���Ͷ�д�豸��ַ+��д�ź�
    iic_SendByte(slave_adr);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //���Ͷ�д�Ĵ�����ַ
    iic_SendByte(reg_adr);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //������ʼ�ź�
    iic_start();
    //���Ͷ�д�豸��ַ+��д�ź�
    iic_SendByte(slave_adr+1);
    //�ȴ���վ��Ӧ
    while(iic_WaitAck()==0);
    //������������
    for(i=0;i<len;i++)
    {
        buffer[i]=iic_ReceiveByte();
        //�ж��Ƿ������һ���ֽ�
        (i==len-1) ? iic_NAck() : iic_Ack();
    }
    iic_stop();

}

//MPU6050ͨѶ���
uint8_t MPU6050_ID_Text(void)
{
    return iic_mpu6050_read_one_byte(MPU6050_ADR,MPU6050_RA_WHO_AM_I);
}


//mpu6050��ʼ��
void MPU6050_Init(void)
{
    HAL_Delay(100);
    //������ߠ�B
    iic_mpu6050_write_one_byte(MPU6050_ADR,MPU6050_RA_PWR_MGMT_1,0x00);
    //���݃x�ɘ��l��
    iic_mpu6050_write_one_byte(MPU6050_ADR,MPU6050_RA_SMPLRT_DIV,0x07);
    iic_mpu6050_write_one_byte(MPU6050_ADR,MPU6050_RA_CONFIG,0x06);
    //���ü��ٶȴ�����������16Gģʽ
    iic_mpu6050_write_one_byte(MPU6050_ADR,MPU6050_RA_ACCEL_CONFIG,0x18);
    //�������Լ�⼰������Χ������ֵ0x18
    iic_mpu6050_write_one_byte(MPU6050_ADR,MPU6050_RA_GYRO_CONFIG,0x18);
}

/*
//��ȡMPU6050���ٶȼ�����
void mpu6050_read_accel_data(uint16_t* buffer)
{
    uint8_t buf[6] = {0};
    iic_mpu6050_read_bytes(MPU6050_ADR,MPU6050_ACC_ADR,buf,6);
    buffer[0] = buf[0]<<8 | buf[1] ;
    buffer[1] = buf[2]<<8 | buf[3] ;
    buffer[2] = buf[4]<<8 | buf[5] ;
}

//��ȡMPU6050����������
void mpu6050_read_gyro_data(uint16_t* buffer)
{
    uint8_t buf[6] = {0};
    iic_mpu6050_read_bytes(MPU6050_ADR,MPU6050_GYRO_ADR,buf,6);
    buffer[0] = buf[0]<<8 | buf[1] ;
    buffer[1] = buf[2]<<8 | buf[3] ;
    buffer[2] = buf[4]<<8 | buf[5] ;
}
*/


//��ȡMPU6050�˲�������
void mpu6050_getdate(uint16_t* mpudate)
{
    uint8_t i;
    uint8_t buffer[12];
    iic_mpu6050_read_bytes(MPU6050_ADR,MPU6050_ACC_ADR,buffer,6);
    iic_mpu6050_read_bytes(MPU6050_ADR,MPU6050_GYRO_ADR,&buffer[6],6);
    for(i=0;i<6;i++)
    {
        mpudate[i]=(((uint16_t)buffer[i<<1]<<8) + buffer[(i<<1)+1]);
        //���ٶȽ���һ�׿������˲�
        if(i<3)
        {
            struct _1_ekf_filter ekf[3]={{0.02,0,8192,0,0.001,0.543},{0.02,0,8192,0,0.001,0.543},{0.02,0,8192,0,0.001,0.543}};
            kalman_1(&ekf[i],(float)mpudate[i]);
            mpudate[i]=(uint16_t)ekf[i].out;
        }
        //���ٶȽ���һ�׵�ͨ�˲�
        else
        {
            uint8_t k=i-3;
            const float factor =0.25f;
            static float tbuf[3];
            mpudate[i]=tbuf[k]=(1-factor)*tbuf[k]+factor*mpudate[i];
        }
    }
}


