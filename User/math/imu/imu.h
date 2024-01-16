#ifndef __IMU_H_
#define __IMU_H_

#include <math.h>
#include "../../application/mpu6050/mpu6050.h"

#define squa( Sq )        (((float)Sq)*((float)Sq))
//转换后的欧拉角
typedef struct 
{
    float roll;
    float pitch;
    float yaw;
}st_ange;

extern st_ange angle;




#endif
