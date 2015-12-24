#include "clock.h"
#include "fan.h"
#include "pwm.h"

void start_fan(uint8_t channel)
{
	stop_fan(channel);
	delay_ms(300);
	set_duty(channel, 0.06);
}

inline void stop_fan(uint8_t channel)
{
	set_duty(channel, 0.05);
}