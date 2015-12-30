#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "encoder.h"
#include "fan.h"
#include "magnet.h"
#include "movement.h"
#include "pwm.h"
#include "switch.h"
#include "usart.h"
#include "utils.h"
#include "push_rod.h"

int main(void)
{
	system_clk_set();
	systick_config();

	uart_config(115200);

	encoder_config();
	switch_config();

	pwm_config();
	
	push_rod_config();

	//mag_up();

	printf("\n\nEntering main loop\n\n");
	while(1)
	{
	}

	return 0;
}
