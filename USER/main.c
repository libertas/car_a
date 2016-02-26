#include <stdio.h>

#include "stm32f4xx.h"

#include "brake.h"
#include "clock.h"
#include "encoder.h"
#include "fan.h"
#include "interpreter.h"
#include "magnet.h"
#include "movement.h"
#include "pwm.h"
#include "switch.h"
#include "usart.h"
#include "utils.h"
#include "push_rod.h"
#include "watchdog.h"

int main(void)
{
	system_clk_set();
	systick_config();

	interpreter_config();
	uart_config(115200);

	brake_config();
	encoder_config();
	switch_config();
	pwm_config();
	watchdog_config();

	printf("\n\nEntering main loop\n\n");
	while(1)
	{
		uprintf(UART5,"tim1 %f \r\n",get_pos_magnet());//pe9/pe11
		uprintf(UART5,"tim2 %f \r\n",get_pos_fan_updown());//pa5/pb3
		uprintf(UART5,"tim9 %f \r\n",get_pos_tim9());//pa2/pa3
		uprintf(UART5,"tim12 %f \r\n",get_pos_mag());//pb14/pb15
		delay_ms(100);
		check_cmd();
	}

	return 0;
}
