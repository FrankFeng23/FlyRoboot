#include "remote.h"


_st_Remote Remote;
_st_ALL_flag ALL_flag;

uint8_t rc_temp[32];

void RC_Angle(void)
{
    static uint16_t cnt_15s=0;
    if(nrf_rx_packet(rc_temp)==1)
    {
        cnt_15s=0;
        uint8_t i;
        uint8_t checksum;
        for(i=0;i<31;i++)
        {
            checksum += rc_temp[i];
        }
        if(checksum==rc_temp[31] && rc_temp[0]==0xAA && rc_temp[1]==0xAF)
        {
            Remote.thr=(uint16_t)rc_temp[4]<<8 | rc_temp[5];
            Remote.roll=(uint16_t)rc_temp[6]<<8 | rc_temp[7];
            Remote.pitch=(uint16_t)rc_temp[8]<<8 | rc_temp[9];
            Remote.yaw=(uint16_t)rc_temp[10]<<8 | rc_temp[11];
            Remote.AUX1=(uint16_t)rc_temp[12]<<8 | rc_temp[13];
            Remote.AUX2=(uint16_t)rc_temp[14]<<8 | rc_temp[15];
            Remote.AUX3=(uint16_t)rc_temp[16]<<8 | rc_temp[17];
            Remote.AUX4=(uint16_t)rc_temp[18]<<8 | rc_temp[19];
            {
                //����ң������
                float roll_pitch_ratio = 0.04f;
                pidRoll.desired=-(Remote.roll-1500)*roll_pitch_ratio;
                pidPitch.desired=-(Remote.pitch-1500)*roll_pitch_ratio;
                if(Remote.yaw>1820)
                {
                    pidYaw.desired -= 0.45f;
                }
                else if(Remote.yaw<1180)
                {
                    pidYaw.desired += 0.45f;
                }
                //�������Ų�Ҫ̫��
                Remote.thr=LIMIT(Remote.thr,1000,2000);
            }
        }
    }
    else        //����15sû���յ������Զ�����
    {
        cnt_15s++;
        if(cnt_15s>5000)
        {
            cnt_15s=0;
            ALL_flag.unlock=0;
            //nrf21l01��ʼ��

        }
    }
    //�жϽ���״̬
    remote_unlock();
}


void remote_unlock(void)
{
    volatile static uint8_t status=WAITING_1;
    static uint16_t cnt=0;
    //�ж��Ƿ��������
    if(Remote.thr<1020 && Remote.yaw<1020)
    {
        status = EXIT_255;
    }
    switch (status)
    {
        //������һ�������������
        case WAITING_1:
            if(Remote.thr<1020)
            {
                status = WAITING_2;
            }
            break;
        //�����ڶ�������������ߣ�����200ms
        case WAITING_2:
            if(Remote.thr>1720)
            {
                static uint8_t cnt=0;
                if(cnt++ > 10)
                {
                    cnt =0;
                    status = WAITING_3;
                }
            }
            break;
        //���������������������
        case WAITING_3:
            if(Remote.thr<1020)
            {
                status = WAITING_4;
            }
            break;
        //����ǰ׼��
        case WAITING_4:
            //����
            ALL_flag.unlock=1;
            //�������״̬
            status=PROCESS_31;
            //����LED�Ƴ���
            LED.status=AlwaysON;
            break;
        //�������״̬
        case PROCESS_31:
            //����������ͳ���6s��������
            if(Remote.thr<1020)
            {
                if (cnt++ >2000)
                {
                    status =EXIT_255;
                } 
            }
            else if(!ALL_flag.unlock)
            {
                status = EXIT_255;
            }
            else
                cnt=0;
            break;
        //��������״̬
        case EXIT_255:
            cnt=0;
            LED.status=Flashing;
            LED.Flashingtime=100;
            ALL_flag.unlock=0;
            status = WAITING_1;
            break;
        default:
            status =EXIT_255;
            break;
    }
}
