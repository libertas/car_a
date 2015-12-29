#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "encoder.h"
#include "fan.h"
#include "movement.h"
#include "pwm.h"
#include "switch.h"
#include "usart.h"
#include "utils.h"

int main(void)
{
	system_clk_set();
	systick_config();

	uart_config(115200);

	encoder_config();
	switch_config();

	pwm_config();

	fan_roll(PI / 3);
/*
	fan_kowtow(PI/4);
	delay_ms(3000);
	fan_kowtow(-PI / 4);
*/

	printf("\n\nEntering main loop\n\n");
	while(1)
	{
	}

	return 0;
}
