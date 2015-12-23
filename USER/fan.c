#include "clock.h"
#include "fan.h"
#include "pwm.h"

void start_fan(void)
{
	set_duty(0, 0.05);
	delay_ms(300);
	set_duty(0, 0.06);
}
