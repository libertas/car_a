#include "automove.h"
#include "clock.h"
#include "debug.h"
#include "encoder.h"
#include "fan.h"
#include "movement.h"
#include "pid.h"
#include "whiteline.h"
#include "can.h"

#define WL_X_MAX 160
uint16_t WL_MAX_SPD = 3000;
uint16_t WL_RUN_SPD = 2000;
uint16_t WL_ROTATE_SPD = 2000;

float wl_x = -1;
float wl_y = -1;
u8 send_array[8];

void set_threshold(uint8_t th)
{
	uint64_t i;
    for(i = 0; i < 100; i++){      
        u8 check_byte = 0x10;
        send_array[0] = check_byte;
        send_array[1] = th;
        check_byte = (check_byte + th) & 0xff;
        send_array[2] = check_byte;
        can_send_msg(COMM_A_ID, send_array, 3);
    }
    
}

void set_wl_value(float x, float y)
{
	wl_x = x;
	wl_y = y;
}

int wl_run(void)
{
	static uint16_t err_count;

	pid_t pr;
	pr.kp = 0.01f;
	pr.kd = 0;
	pr.ki = 0;
	float prout;
	float spd_r;
	
	while(1) {
		if(stop_flag) {
			stop();
			return 0;
		} else if(0 < wl_x && 0 <= wl_y) {
			WL_MAX_SPD = WL_RUN_SPD = 1000;
			WL_ROTATE_SPD = 500;
			pr.set_value = 20;
			set_threshold(250);
			
			pr.actual_value = wl_x - WL_X_MAX / 2;
			prout = pid_realize(&pr);
			
			#ifdef DEBUG_WL
			printf("prout:%f\n", prout);
			#endif
			
			spd_r = prout * WL_ROTATE_SPD;

			arg_speeds[0] = -VECT_W0 * spd_r;
			arg_speeds[1] = VECT_W1 * spd_r;
			arg_speeds[2] = -VECT_W2 * spd_r;
			arg_speeds[3] = VECT_W3 * spd_r;
			
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
			
			err_count = 0;
			
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
			
			if(err_count >= 1000) {
				stop();
			} else {
				err_count++;
			}
			// return 0;
		}
	}
}
