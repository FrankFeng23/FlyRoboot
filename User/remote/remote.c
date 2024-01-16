#include "remote.h"


_st_Remote Remote;
_st_ALL_flag ALL_flag;

uint8_t rc_temp[32];

void RC_Angle(void)
{
    if(nrf_rx_packet(rc_temp)==1)
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
            //处理遥控数据
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
            
        }
    }

}


void remote_unlock(void)
{
    volatile static uint8_t status=WAITING_1;
    static uint16_t cnt=0;
    //判断是否进入锁定
    if(Remote.thr<1020 && Remote.yaw<1020)
    {
        status = EXIT_255;
    }
    switch (status)
    {
        //解锁第一步油门拉到最低
        case WAITING_1:
            if(Remote.thr<1020)
            {
                status = WAITING_2;
            }
            break;
        //解锁第二步油门拉到最高，持续200ms
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
        //解锁第三步油门拉到最低
        case WAITING_3:
            if(Remote.thr<1020)
            {
                status = WAITING_4;
            }
            break;
        //解锁前准备
        case WAITING_4:
            //上锁
            ALL_flag.unlock=1;
            //进入控制状态
            status=PROCESS_31;
            //控制LED灯常亮

            break;
        //进入控制状态
        case PROCESS_31:
            //油门拉至最低持续6s进入锁定
            if(Remote.thr<1020)
            {
                if (cnt++ >2000)
                {
                    status =EXIT_255;
                }
                
            }
            else
                cnt=0;
            break;
        case EXIT_255:
            ALL_flag.unlock=0;
            status = WAITING_1;

            break;
        default:
            status =EXIT_255;
            break;
    }
}
