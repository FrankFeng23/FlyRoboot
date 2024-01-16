#include "motor.h"

PidObject* (pPidObject[]) = {&pidRateX,&pidRateY,&pidRateZ,&pidPitch,&pidYaw,&pidRoll};


void FlyPidControl(float dt)
{
    volatile static uint8_t status=WAITING_1;
    switch(status)
    {
        case WAITING_1 :


            break;
        case READY_11 :
        
            //PID参数复位
            pidreset(pPidObject,6);

            break;
        case PROCESS_31:

            //三轴角速度实测值
            pidRateX.measured=mpu6050_data.gyro_x;
            pidRateY.measured=mpu6050_data.gyro_y;
            pidRateZ.measured=mpu6050_data.gyro_z;
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


            break;
        default:

            break;
    }
}