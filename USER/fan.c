#include "brake.h"
#include "clock.h"
#include "encoder.h"
#include "fan.h"
#include "movement.h"
#include "pwm.h"

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

void fan_kowtow(float rad)
{
	if(rad > PI / 2)
		rad = PI / 2;
	else if(rad < -1 * PI / 2)
		rad = -1 * PI / 2;

	if(rad - fan_kowtow_rad < ZERO) {
		return;
	} else if(rad > fan_kowtow_rad) {
		kowtow_dir = 1;
		set_duty(0, 0.74);
	} else {
		kowtow_dir = 0;
		set_duty(0, 0.67);
	}
	fan_kowtow_rad = rad;
}

void fan_kowtow_stop(void)
{
	set_duty(0, 0.71);
}

void kowtow_check(void)
{
	switch(kowtow_dir) {
		case 0:
			if(get_pos_fan() < fan_kowtow_rad) {
				fan_kowtow_stop();
			}
			break;
		case 1:
			if(get_pos_fan() > fan_kowtow_rad) {
				fan_kowtow_stop();
			}
			break;
		case 0xff:
			break;
		default:
			break;
	}
}

void fan_up(void)
{
	brake_release(0);
	set_duty(2, 0.068);
}

void fan_down(void)
{
	brake_release(0);
	set_duty(2, 0.074);
}

void stop_fan_up_down(void)
{
	set_duty(2, 0.071);
	brake(0);
}
