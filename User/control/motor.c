#include "motor.h"

PidObject* (pPidObject[]) = {&pidRateX,&pidRateY,&pidRateZ,&pidRoll,&pidPitch,&pidYaw};

void FlyPidControl(float dt)
{
    volatile static uint8_t status=WAITING_1;
    switch(status)
    {
        case WAITING_1 :
            if(ALL_flag.unlock)
            {
                status=READY_11;
            }
            break;
        case READY_11 :
            //PID参数复位
            pidreset(pPidObject,6);
            status=PROCESS_31;
            break;
        case PROCESS_31:
            //三轴角速度实测值
            pidRateX.measured=mpu6050_data.gyro_x*Gyro_G;
            pidRateY.measured=mpu6050_data.gyro_y*Gyro_G;
            pidRateZ.measured=mpu6050_data.gyro_z*Gyro_G;
            //欧拉角转换值（实际值）
            pidRoll.measured=angle.roll;
            pidPitch.measured=angle.pitch;
            pidYaw.measured=angle.yaw;

            //翻滚角PID调节
            Cascadepid(&pidRoll,&pidRateX,dt);
            //俯仰角PID调节
            Cascadepid(&pidPitch,&pidRateY,dt);
            //偏航角PID调节
            Cascadepid(&pidYaw,&pidRateZ,dt);
            break;
        case EXIT_255 :
            pidreset(pPidObject,6);
            status=WAITING_1;
            break;
        default:
            status=EXIT_255;
            break;
    }
}

uint16_t motor[4];
uint16_t low_thr_cnt;
void MotorControl(void)
{
    volatile static uint8_t status=WAITING_1;
    switch (status)
    {
    case WAITING_1:
        if(ALL_flag.unlock)
        {
            motor[0]=motor[1]=motor[2]=motor[3]=0;
            status=WAITING_2;
        }
        break;
    case WAITING_2:
        if(Remote.thr>1100)
        {
            pidreset(pPidObject,6);
            low_thr_cnt=0;
            status=PROCESS_31;
        }
        break;
    case PROCESS_31:
        uint16_t temp;
        temp=Remote.thr-1000;
        if(temp<10)
        {
            low_thr_cnt++;
            if(low_thr_cnt>500)
            {
                motor[0]=motor[1]=motor[2]=motor[3]=0;
                pidreset(pPidObject,6);
                status=WAITING_2;
            }
        }
        low_thr_cnt=0;
        motor[0]=motor[1]=motor[2]=motor[3]=LIMIT(temp,0,800);
        motor[0]+= + pidRateX.out + pidRateY.out + pidRateZ.out;//; 姿态输出分配给各个电机的控制量
		motor[1]+= - pidRateX.out + pidRateY.out - pidRateZ.out ;//;
		motor[2]+= + pidRateX.out - pidRateY.out - pidRateZ.out;
		motor[3]+= - pidRateX.out - pidRateY.out + pidRateZ.out;//;
        break;
    case EXIT_255:
        motor[0]=motor[1]=motor[2]=motor[3]=LIMIT(temp,0,800);
        status=WAITING_1;
        break;
    default:
        break;
    }
}