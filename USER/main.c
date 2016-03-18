#include <stdio.h>

#include "stm32f4xx.h"

#include "auto_control.h"
#include "brake.h"
#include "clock.h"
#include "database.h"
#include "encoder.h"
#include "fan.h"
#include "flash.h"
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
	// encoder_config();
	// switch_config();
	pwm_config();
	// watchdog_config();
	
	db_init();
	uint8_t data[2] = {0xab, 0xcd};
	uint8_t ndata[2];
	db_save("test0", data, 2);
	db_sync();
	db_read("test0", ndata);
	printf("%x %x\n", data[0], data[1]);


	printf("\n\nEntering main loop\n\n");
	while(1)
	{
		check_cmd();
	}

	return 0;
}
