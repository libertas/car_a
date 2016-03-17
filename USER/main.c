#include <stdio.h>

#include "stm32f4xx.h"

#include "auto_control.h"
#include "brake.h"
#include "clock.h"
#include "encoder.h"
#include "fan.h"
#include "interpreter.h"
#include "magnet.h"
#include "movement.h"
#include "multi_processing.h"
#include "pwm.h"
#include "switch.h"
#include "usart.h"
#include "us100.h"
#include "utils.h"
#include "push_rod.h"
#include "spi.h"
#include "suart.h"
#include "watchdog.h"

int main(void)
{
	system_clk_set();
	systick_config();

	interpreter_config();
	uart_config(115200);
	spi_config();
	suart_config();

	brake_config();
	encoder_config();
	// switch_config();
	pwm_config();
	watchdog_config();
	
	#ifdef MP_MASTER
	delay_ms(1000);
	for(uint8_t i = 0; i < 0xff; i++) {
		printf("%x\n", spi_wr(i));
	}
	#endif
	

	printf("\n\nEntering main loop\n\n");
	while(1)
	{
		check_cmd();
	}

	return 0;
}
