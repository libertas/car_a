#include "brake.h"
#include "clock.h"
#include "debug.h"
#include "encoder.h"
#include "fan.h"
#include "movement.h"
#include "pwm.h"
#include "utils.h"

void start_fan(void)
{
	stop_fan();
	delay_ms(300);
	set_duty(FAN_CHANNEL, 0.06);
}

inline void stop_fan(void)
{
	set_duty(FAN_CHANNEL, 0.05);
}

float fan_kowtow_rad = 0;

/*
	0xff means not working
	1 means going forwards
	0 means going backwards
*/
uint8_t kowtow_dir = 0xff;

void fan_roll(float rad)
{
	set_duty(6, (float)((float)0.11 - (float)0.05 * rad / ((float)PI / 2)));
	
	#ifdef DEBUG
	printf("\nfan_roll(%f)\n", rad);
	#endif
}

void fan_roll_r(int8_t dir)
{
	static float current_rad;
	current_rad += dir * FAN_RMOVE_DIST;
	fan_roll(current_rad);
}

void fan_kowtow(float rad)
{
	float pos = get_pos_fan();

	#ifdef DEBUG
	printf("\nfan_kowtow(%f)\n", rad);
	#endif

	if(ABS(rad - pos) < ZERO) {
		
		#ifdef DEBUG
		printf("\nNot kowtow happened\n");
		#endif
		
		return;
	} else if(rad > pos) {
		
		#ifdef DEBUG
		printf("\nKowtow:dir=1\n");
		#endif
		
		kowtow_dir = 1;
		set_duty(FAN_KOWTOW_CHANNEL, 0.066);
	} else {
		
		#ifdef DEBUG
		printf("\nKowtow:dir=0\n");
		#endif
		
		kowtow_dir = 0;
		set_duty(FAN_KOWTOW_CHANNEL, 0.075);
	}

	brake_release(2);

	fan_kowtow_rad += rad;
}

void fan_kowtow_r(int8_t dir)
{
	fan_kowtow(get_pos_fan() + dir * FAN_RMOVE_DIST);
}

void fan_kowtow_stop(void)
{
	set_duty(FAN_KOWTOW_CHANNEL, 0.71);
	brake(2);
	
	#ifdef DEBUG
	printf("\nfan_kowtow_stop()\n");
	#endif
}

void kowtow_check(void)
{
	#ifdef DEBUG_POS_FAN
	printf("pos:%f\n", get_pos_fan());
	#endif
	
	switch(kowtow_dir) {
		default:
			break;
		case 0xff:
			break;
		case 0:
			if(get_pos_fan() < fan_kowtow_rad) {
				fan_kowtow_stop();
				kowtow_dir = 0xff;
			}
			break;
		case 1:
			if(get_pos_fan() > fan_kowtow_rad) {
				fan_kowtow_stop();
				kowtow_dir = 0xff;
			}
			break;
	}
}

void fan_up(void)
{
	brake_release(0);
	set_duty(FAN_UPDOWN_CHANNEL, 0.067);
}

void fan_down(void)
{
	brake_release(0);
	set_duty(FAN_UPDOWN_CHANNEL, 0.074);
}

void stop_fan_up_down(void)
{
	set_duty(FAN_UPDOWN_CHANNEL, 0.071);
	brake(0);
}
