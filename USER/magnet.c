#include "magnet.h"
#include "pwm.h"

void mag_in(void)
{
	set_duty(MAG_IN_CHANNEL, 0.10);
}

void mag_out(void)
{
	set_duty(MAG_IN_CHANNEL, 0.001);
}

void mag_in_stop(void)
{
}

void mag_up(void)
{
	set_duty(MAG_UP_CHANNEL, 0.076);
}

void mag_up_stop(void)
{
	set_duty(MAG_UP_CHANNEL, 0.071);
}
