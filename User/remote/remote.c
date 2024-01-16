#include "remote.h"


_st_Remote Remote;

uint8_t rc_temp[32];

void RC_Angle(void)
{
    if(nrf_rx_packet(rc_temp)==1)
    {
        Remote.thr=(uint16_t)rc_temp[4]<<8 | rc_temp[5];
        Remote.roll=(uint16_t)rc_temp[6]<<8 | rc_temp[7];
        Remote.pitch=(uint16_t)rc_temp[8]<<8 | rc_temp[9];
        Remote.yaw=(uint16_t)rc_temp[10]<<8 | rc_temp[11];
        Remote.AUX1=(uint16_t)rc_temp[12]<<8 | rc_temp[13];
        Remote.AUX2=(uint16_t)rc_temp[14]<<8 | rc_temp[15];
        Remote.AUX3=(uint16_t)rc_temp[16]<<8 | rc_temp[17];
        Remote.AUX4=(uint16_t)rc_temp[18]<<8 | rc_temp[19];
    }

}
