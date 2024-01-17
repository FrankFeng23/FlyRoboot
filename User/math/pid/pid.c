#include "pid.h"

PidObject pidRateX;
PidObject pidRateY;
PidObject pidRateZ;

PidObject pidPitch;
PidObject pidRoll;
PidObject pidYaw;

void pidreset(PidObject** ppid,uint8_t len)
{
    int i;
    for(i=0;i<len;i++)
    {
        ppid[i]->integ=0;
        ppid[i]->offset=0;
        ppid[i]->out=0;
        ppid[i]->prevError=0;

    }
}
void pidupdate(PidObject* pid ,const float dt)
{
    float error;
    float deriv;
    //计算当前时刻误差
    error = pid->desired-pid->measured+pid->offset;
    //误差积分累加
    pid->integ += error *dt;
    //前后两次误差微分
    deriv=(error-pid->prevError)/dt;
    //计算PID输出
    pid->out=pid->kp*error+pid->ki*pid->integ+pid->kd*deriv;
    //保存上次误差值
    pid->prevError=error;
}


void Cascadepid(PidObject* pidw,PidObject* pidn,const float dt)
{
    pidupdate(pidw,dt);
    pidn->desired=pidw->out;
    pidupdate(pidn,dt);
}
