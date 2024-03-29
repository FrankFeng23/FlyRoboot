#ifndef __PID_H_
#define __PID_H_
#include "stm32f1xx.h"

#define LIMIT(x,min,max)        (x < min ? min : (x > max ? max : x))
typedef volatile struct
{
	float desired;     //< set point
	float offset;      //
	float prevError;    //< previous error
	float integ;        //< integral
	float kp;           //< proportional gain
	float ki;           //< integral gain
	float kd;           //< derivative gain
	float IntegLimitHigh;       //< integral limit
	float IntegLimitLow;
	float measured;
	float out;
	float OutLimitHigh;
	float OutLimitLow;
}PidObject;



void pidupdate(PidObject* pid ,const float dt);
void Cascadepid(PidObject* pidw,PidObject* pidn,float dt);
void pidreset(PidObject** ppid,uint8_t len);

extern	PidObject pidRateX;
extern	PidObject pidRateY;
extern	PidObject pidRateZ;

extern	PidObject pidPitch;
extern	PidObject pidRoll;
extern	PidObject pidYaw;

#endif
