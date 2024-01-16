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
        
            //PID������λ
            pidreset(pPidObject,6);

            break;
        case PROCESS_31:

            //������ٶ�ʵ��ֵ
            pidRateX.measured=mpu6050_data.gyro_x;
            pidRateY.measured=mpu6050_data.gyro_y;
            pidRateZ.measured=mpu6050_data.gyro_z;
            //ŷ����ת��ֵ��ʵ��ֵ��
            pidRoll.measured=angle.roll;
            pidPitch.measured=angle.pitch;
            pidYaw.measured=angle.yaw;

            //������PID����
            Cascadepid(&pidRoll,&pidRateX,dt);
            //������PID����
            Cascadepid(&pidPitch,&pidRateY,dt);
            //ƫ����PID����
            Cascadepid(&pidYaw,&pidRateZ,dt);

            break;
        case EXIT_255 :


            break;
        default:

            break;
    }
}