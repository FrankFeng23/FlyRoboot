#ifndef __REMOTE_H_
#define __REMOTE_H_

#include "stm32f1xx.h"
#include "../application/nrf24l01/nrf24l01.h"


//接收模拟数据
typedef struct
{
	uint16_t roll;
	uint16_t pitch;
	uint16_t thr;
	uint16_t yaw;
	uint16_t AUX1;
	uint16_t AUX2;
	uint16_t AUX3;
	uint16_t AUX4;	
}_st_Remote;

extern _st_Remote Remote;













#endif