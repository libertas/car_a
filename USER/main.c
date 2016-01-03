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
	push_rod_config();

//	for(int i = 0; i < 20; i++){
//		move_xy_c((int8_t) 0, (int8_t) -6 * i);
//		delay_ms(100);
//	}
	//uprintf(USART1,"\r0V-245\r1V435\r2V435\r");


	printf("\n\nEntering main loop\n\n");
	while(1)
	{
		check_cmd();
	}

	return 0;
}
