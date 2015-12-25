#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "encoder.h"
#include "movement.h"
#include "pwm.h"
#include "switch.h"
#include "usart.h"

int main(void)
{
	systick_config();

	uart_config(115200);

	encoder_config();
	switch_config();

	pwm_config();
	
	//move_up();

	printf("\n\nEntering main loop\n\n");
	while(1)
	{
		//printf("test usart,ten ms\n");
		//delay_ms(10);
	}

	return 0;
}
