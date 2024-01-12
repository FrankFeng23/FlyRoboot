#include "motor.h"

PidObject* (pPidObject[]) = {&pidRateX,&pidRateY,&pidRateZ,&pidPitch,&pidYaw,&pidRoll};


void FlyPidControl(void)
{
    volatile static uint8_t status=WAITING_1;
    switch(status)
    {
        case WAITING_1 :


            break;
        case READY_11 :


            break;
        case PROCESS_31:


            break;
        case EXIT_255 :


            break;
        default:

            break;
    }
}