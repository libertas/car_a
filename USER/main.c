#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "encoder.h"
#include "hpwm.h"
#include "movement.h"
#include "pwm.h"
#include "switch.h"
#include "usart.h"

int main(void)
{
	systick_config();

	uart1_config(115200);
	uart2_config(115200);
	uart3_config(115200);
	uart4_config(115200);

	encoder_config();
	switch_config();

	// pwm_config();
	
	hpwm_config(500,168);
	TIM_SetCompare1(TIM1,200);
	TIM_SetCompare2(TIM1,100);
	TIM_SetCompare3(TIM1,150);
	TIM_SetCompare4(TIM1,250);
	
	move_up();
	delay_ms(10000);
	stop_move_up();

	printf("\n\nEntering main loop\n\n");
	while(1)
	{
	}

	return 0;
}
