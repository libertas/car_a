#include "brake.h"
#include "clock.h"
#include "fan.h"
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
