#include "automove.h"
#include "clock.h"
#include "debug.h"
#include "movement.h"
#include "pid.h"
#include "whiteline.h"

#define WL_X_MAX 160
uint16_t WL_MAX_SPD = 3000;
uint16_t WL_RUN_SPD = 2000;
uint16_t WL_ROTATE_SPD = 2000;

float wl_x = -1;
float wl_y = -1;

void set_threshold(uint8_t th)
{
	uint64_t i;
	for(i = 0; i < 100; i++) {
		USART_SendData(USART3, 0x10);
		USART_SendData(USART3, th);
		USART_SendData(USART3, (0x10 + th) & 0xff);
	}
}

void set_wl_value(float x, float y)
{
	wl_x = x;
	wl_y = y;
}

int wl_run(void)
{	
	pid_t pr;
	pr.kp = 0.01f;
	pr.kd = 0;
	pr.ki = 0;
	float prout;

	while(1) {
		if(0 < wl_x && 0 <= wl_y) {
			
			if(get_gps_y() > 7.0f) {
				 if(get_gps_x() > 4.5f) {
					WL_MAX_SPD = WL_RUN_SPD = 1000;
					WL_ROTATE_SPD = 500;
					pr.set_value = 15;
					set_threshold(250);
				} else if(get_gps_x() > 3.0f) {
					WL_MAX_SPD = WL_RUN_SPD = 3000;
					WL_ROTATE_SPD = 2000;
					pr.set_value = 15;
					set_threshold(250);
				} else {
					WL_MAX_SPD = WL_RUN_SPD = 3000;
					WL_ROTATE_SPD = 2000;
					pr.set_value = 0;
					set_threshold(220);
				}
			} else {
				WL_MAX_SPD = WL_RUN_SPD = 2500;
				WL_ROTATE_SPD = 2000;
				pr.set_value = 15;
				set_threshold(240);
			}
			pr.actual_value = wl_x - WL_X_MAX / 2;
			prout = pid_realize(&pr);
			
			#ifdef DEBUG_WL
			printf("prout:%f\n", prout);
			#endif
			
			float spd_r = prout * WL_ROTATE_SPD;

			arg_speeds[0] = VECT_W0 * spd_r;
			arg_speeds[1] = VECT_W1 * spd_r;
			arg_speeds[2] = -VECT_W2 * spd_r;
			arg_speeds[3] = -VECT_W3 * spd_r;
			
			uprintf(USART1, "\rAC10000\rDEC500\r");
			

			for(uint8_t i = 0; i < 4; i++) {
				if(arg_speeds[i] > 0) {
					if(arg_speeds[i] > WL_MAX_SPD) {
						arg_speeds[i] = WL_MAX_SPD;
					}
				} else if(arg_speeds[i] < 0) {
					if(arg_speeds[i] < -WL_MAX_SPD) {
						arg_speeds[i] = -WL_MAX_SPD;
					}
				}
			}
			
			arg_speeds[0] += VECT_W0 * WL_RUN_SPD;
			arg_speeds[1] += VECT_W1 * WL_RUN_SPD;
			arg_speeds[2] += VECT_W2 * WL_RUN_SPD;
			arg_speeds[3] += VECT_W3 * WL_RUN_SPD;

			uprintf(USART1,\
				"\r0V%d\r1V%d\r2V%d\r5V%d\r",\
				arg_speeds[0],\
				arg_speeds[1],\
				arg_speeds[2],\
				arg_speeds[3]\
				);
			
			#ifdef DEBUG_WL
			printf("wl_x= %f\twl_y= %f\n", wl_x, wl_y);
			printf("arg_speeds:\n");
			printf("\t0:%d\n", arg_speeds[0]);
			printf("\t1:%d\n", arg_speeds[1]);
			printf("\t2:%d\n", arg_speeds[2]);
			printf("\t3:%d\n", arg_speeds[3]);
			#endif
			
			// delay_ms(30);
		} else {
			#ifdef DEBUG_WL
			printf("wl_x= %f\twl_y= %f\n", wl_x, wl_y);
			#endif
			
			stop();
			// return 0;
		}
	}
}