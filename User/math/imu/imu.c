#include "imu.h"

st_ange angle;
const float Gyro_G = 0.03051756f*2;	  	//�����ǳ�ʼ������+-2000��ÿ����1 / (65536 / 4000) = 0.03051756*2		
const float Gyro_Gr = 0.0005326f*2;     //������ÿ��,ת������ÿ���� 2*0.03051756	 * 0.0174533f = 0.0005326*2
const float RtA = 57.2957795f;


static float NormAcc;
 
typedef volatile struct {
  float q0;
  float q1;
  float q2;
  float q3;
} Quaternion;
 Quaternion NumQ = {1, 0, 0, 0};
 
	 struct V{
				float x;
				float y;
				float z;
				};
	
volatile	 struct V GyroIntegError = {0}; 
	 
void imu_rest(void)
{
	NumQ.q0 =1;
	NumQ.q1 = 0;
	NumQ.q2 = 0;
	NumQ.q3 = 0;	
	GyroIntegError.x = 0;
	GyroIntegError.y = 0;
	GyroIntegError.z = 0;
	angle.pitch = 0;
	angle.roll = 0;
}


void GetAngle(const st_mpu *pMpu,st_ange *pAngE, float dt) 
{		
	volatile struct V Gravity,Acc,Gyro,AccGravity;

	static  float KpDef = 0.5f ;
	static  float KiDef = 0.0001f;
//	static Quaternion NumQ = {1, 0, 0, 0};
	float q0_t,q1_t,q2_t,q3_t;
  //float NormAcc;	
	float NormQuat; 
	float HalfTime = dt * 0.5f;

	

	// ��ȡ��Ч��ת�����е��������� 
	Gravity.x = 2*(NumQ.q1 * NumQ.q3 - NumQ.q0 * NumQ.q2);								
	Gravity.y = 2*(NumQ.q0 * NumQ.q1 + NumQ.q2 * NumQ.q3);						  
	Gravity.z = 1-2*(NumQ.q1 * NumQ.q1 + NumQ.q2 * NumQ.q2);	
	// ���ٶȹ�һ��
    NormAcc = 1/sqrt(squa(mpu6050_data.acc_x)+ squa(mpu6050_data.acc_y) +squa(mpu6050_data.acc_z));
	
    Acc.x = pMpu->acc_x * NormAcc;
    Acc.y = pMpu->acc_y * NormAcc;
    Acc.z = pMpu->acc_z * NormAcc;	
 	//������˵ó���ֵ
	AccGravity.x = (Acc.y * Gravity.z - Acc.z * Gravity.y);
	AccGravity.y = (Acc.z * Gravity.x - Acc.x * Gravity.z);
	AccGravity.z = (Acc.x * Gravity.y - Acc.y * Gravity.x);
	//�������ٶȻ��ֲ������ٶȵĲ���ֵ
    GyroIntegError.x += AccGravity.x * KiDef;
    GyroIntegError.y += AccGravity.y * KiDef;
    GyroIntegError.z += AccGravity.z * KiDef;
	//���ٶ��ںϼ��ٶȻ��ֲ���ֵ
    Gyro.x = pMpu->gyro_x * Gyro_Gr + KpDef * AccGravity.x  +  GyroIntegError.x;//������
    Gyro.y = pMpu->gyro_y * Gyro_Gr + KpDef * AccGravity.y  +  GyroIntegError.y;
    Gyro.z = pMpu->gyro_z * Gyro_Gr + KpDef * AccGravity.z  +  GyroIntegError.z;		
	// һ�����������, ������Ԫ��

	q0_t = (-NumQ.q1*Gyro.x - NumQ.q2*Gyro.y - NumQ.q3*Gyro.z) * HalfTime;
	q1_t = ( NumQ.q0*Gyro.x - NumQ.q3*Gyro.y + NumQ.q2*Gyro.z) * HalfTime;
	
	
	q2_t = ( NumQ.q3*Gyro.x + NumQ.q0*Gyro.y - NumQ.q1*Gyro.z) * HalfTime;
	q3_t = (-NumQ.q2*Gyro.x + NumQ.q1*Gyro.y + NumQ.q0*Gyro.z) * HalfTime;
	
	NumQ.q0 += q0_t;
	NumQ.q1 += q1_t;
	NumQ.q2 += q2_t;
	NumQ.q3 += q3_t;
	// ��Ԫ����һ��
	NormQuat = 1/sqrt(squa(NumQ.q0) + squa(NumQ.q1) + squa(NumQ.q2) + squa(NumQ.q3));
	NumQ.q0 *= NormQuat;
	NumQ.q1 *= NormQuat;
	NumQ.q2 *= NormQuat;
	NumQ.q3 *= NormQuat;	
	

		// ��Ԫ��תŷ����
	{
		 
			#ifdef	YAW_GYRO
			*(float *)pAngE = atan2f(2 * NumQ.q1 *NumQ.q2 + 2 * NumQ.q0 * NumQ.q3, 1 - 2 * NumQ.q2 *NumQ.q2 - 2 * NumQ.q3 * NumQ.q3) * RtA;  //yaw
			#else
				float yaw_G = pMpu->gyro_z * Gyro_G;
				if((yaw_G > 1.2f) || (yaw_G < -1.2f)) //����̫С������Ϊ�Ǹ��ţ�����ƫ������
				{
					pAngE->yaw  += yaw_G * dt;			
				}
			#endif
			pAngE->pitch  =  asin(2 * NumQ.q0 *NumQ.q2 - 2 * NumQ.q1 * NumQ.q3) * RtA;						
		
			pAngE->roll	= atan2(2 * NumQ.q2 *NumQ.q3 + 2 * NumQ.q0 * NumQ.q1, 1 - 2 * NumQ.q1 *NumQ.q1 - 2 * NumQ.q2 * NumQ.q2) * RtA;	//PITCH 			
	}
}
