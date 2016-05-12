#include <stm32f4xx_gpio.h>

#include "car.h"
#include "clock.h"
#include "magnet.h"
#include "pwm.h"

#ifdef CAR_A_1
void mag_in(void)
{
	set_duty(MAG_IN_CHANNEL, 0.103);
}

void mag_out(void)
{
	set_duty(MAG_IN_CHANNEL, 0.045);
}
#endif

#ifdef CAR_A_2
void mag_in(void)
{
	//wait for dianji
}

void mag_out(void)
{
	//wait for dianji
}
#endif

void mag_near(void)
{
	set_duty(MAG_NEAR_CHANNEL, 0.09);
}

void mag_far(void)
{
	set_duty(MAG_NEAR_CHANNEL, 0.05);
}
