#include "automove.h"
#include "clock.h"
#include "debug.h"
#include "movement.h"
#include "whiteline.h"

#define WL_X_MAX 180
#define WL_X_ERR 10
#define WL_ERR_SPD 10
#define WL_MAX_SPD 2000
#define WL_RUN_SPD 1000

float wl_x = -1;
float wl_y = -1;

void set_wl_value(float x, float y)
{
	wl_x = x;
	wl_y = y;
}

void wl_run(void)
{
	arg_speeds[0] = VECT_W0 * WL_RUN_SPD;
	arg_speeds[1] = VECT_W1 * WL_RUN_SPD;
	arg_speeds[2] = VECT_W2 * WL_RUN_SPD;
	arg_speeds[3] = VECT_W3 * WL_RUN_SPD;

	while(1) {
		if(-1 != wl_x && -1 != wl_y) {
			if(wl_x < WL_X_MAX / 2 - WL_X_ERR) {
				arg_speeds[0] += VECT_W0 * WL_ERR_SPD;
				arg_speeds[1] += VECT_W0 * WL_ERR_SPD;
				arg_speeds[2] += -VECT_W0 * WL_ERR_SPD;
				arg_speeds[3] += -VECT_W0 * WL_ERR_SPD;
			}
			if(wl_x > WL_X_MAX / 2 + WL_X_ERR) {
				arg_speeds[0] += -VECT_W0 * WL_ERR_SPD;
				arg_speeds[1] += -VECT_W0 * WL_ERR_SPD;
				arg_speeds[2] += VECT_W0 * WL_ERR_SPD;
				arg_speeds[3] += VECT_W0 * WL_ERR_SPD;
			}
			
			uprintf(USART1, "\rAC10000\rDEC50\r");
			

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
			
			delay_ms(30);
		} else {
			#ifdef DEBUG_WL
			printf("wl_x= %f\twl_y= %f\n", wl_x, wl_y);
			#endif
			
			stop();
		}
	}
}
