#include "MTi.h"
#include "usart.h"

void mti(void)
{
	int a,b,c;
	
	if(buffer[0] == flag[0])
		if(buffer[1] == flag[1])
			if(buffer[2] == flag[2])
				if(buffer[3] == flag[3]){
					a = btol(buffer + 4);
					b = btol(buffer + 8);
					c = btol(buffer + 12);
					memcpy(&roll, &a, sizeof(a));
					memcpy(&pitch, &b, sizeof(b));
					memcpy(&yaw, &c, sizeof(c));
					uprintf(USART2, "roll = %f\t pitch = %f\t yaw = %f\t\r\n",roll, pitch, yaw);
				}
}

int btol(uint8_t buffer[])
{
	return 	( ((((int)buffer[0]<<24) | ((int)buffer[1]<<16)) | ((int)buffer[2]<<8)) | ((int)buffer[3]));
}
