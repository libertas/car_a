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
/*
        *mag_in gpio_config in pwm_config
        mag_in control gpio
        pa10		pa10
        1		0	in
        0		1	out
        0		0	stop
*/
void mag_in(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);
	set_duty(MAG_IN_CHANNEL, 0.9);
	delay_ms(50);
	//STOP
	GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);
	set_duty(MAG_IN_CHANNEL, 0);
}

void mag_out(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET);
	set_duty(MAG_IN_CHANNEL, 0.9);
	delay_ms(50);
	//STOP
	GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);
	set_duty(MAG_IN_CHANNEL, 0);
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
