#ifndef __PID_H_
#define __PID_H_

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






#endif
