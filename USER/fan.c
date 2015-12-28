#include "brake.h"
#include "clock.h"
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

float g_fan_kowtow_rad = 0;

void fan_kowtow(float rad)
{
	if(rad > PI / 2)
		rad = PI / 2;
	else if(rad < -1 * PI / 2)
		rad = -1 * PI / 2;

	if(rad - g_fan_kowtow_rad < ZERO) {
		return;
	} else if(rad > g_fan_kowtow_rad) {
		set_duty(0, 0.74);
	} else {
		set_duty(0, 0.67);
	}
	g_fan_kowtow_rad = rad;
}

void fan_kowtow_stop(void)
{
	set_duty(0, 0.71);
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
