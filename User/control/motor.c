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
        motor[0]+= + pidRateX.out + pidRateY.out + pidRateZ.out;//; ��̬����������������Ŀ�����
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