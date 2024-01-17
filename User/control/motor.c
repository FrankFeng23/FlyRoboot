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


void MotorControl(void)
{

}