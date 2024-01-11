#include "kalman.h"


void kalman_1(struct _1_ekf_filter *ekf,float input)  //Ò»Î¬¿¨¶ûÂü
{
	ekf->Now_P = ekf->LastP + ekf->Q;
	ekf->Kg = ekf->Now_P / (ekf->Now_P + ekf->R);
	ekf->out = ekf->out + ekf->Kg * (input - ekf->out);
	ekf->LastP = (1-ekf->Kg) * ekf->Now_P ;
}
