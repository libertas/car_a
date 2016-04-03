#include "mti.h"
#include "usart.h"

const uint8_t mti_flag[4] = {0xFA,0xFF,0x32,0x0E};
float mti_roll = 0;
float mti_pitch = 0;
float mti_yaw = 0;


float mti(void)
{
	int a,b,c;
	
	if(mti_buffer[0] == mti_flag[0])
		if(mti_buffer[1] == mti_flag[1])
			if(mti_buffer[2] == mti_flag[2])
				if(mti_buffer[3] == mti_flag[3]){
					a = btol(mti_buffer + 4);
					b = btol(mti_buffer + 8);
					c = btol(mti_buffer + 12);
					memcpy(&mti_roll, &a, sizeof(a));
					memcpy(&mti_pitch, &b, sizeof(b));
					memcpy(&mti_yaw, &c, sizeof(c));
					mti_roll *= ANGLE_TO_RAD;
					mti_pitch *= ANGLE_TO_RAD;
					mti_yaw *= ANGLE_TO_RAD;
					//uprintf(UART5, "roll = %f\t pitch = %f\t yaw = %f\t\r\n",mti_roll, mti_pitch, mti_yaw);
				}
	return mti_yaw;
}

int btol(uint8_t buffer[])
{
	return 	( ((((int)buffer[0]<<24) | ((int)buffer[1]<<16)) | ((int)buffer[2]<<8)) | ((int)buffer[3]));
}
