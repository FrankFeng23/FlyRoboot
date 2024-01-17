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
            //PID������λ
            pidreset(pPidObject,6);
            status=PROCESS_31;
            break;
        case PROCESS_31:
            //������ٶ�ʵ��ֵ
            pidRateX.measured=mpu6050_data.gyro_x*Gyro_G;
            pidRateY.measured=mpu6050_data.gyro_y*Gyro_G;
            pidRateZ.measured=mpu6050_data.gyro_z*Gyro_G;
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