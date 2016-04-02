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
#include "mti.h"
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
#include "dma.h"

int main(void)
{
	system_clk_set();
	systick_config();
	db_init();

	interpreter_config();
	uart_config(115200);
	dma_config();
	spi_config();
	suart_config();

	brake_config();
	encoder_config();
	switch_config();
	push_rod_config();
	pwm_config();
	// watchdog_config();

	//move_arc(2, PI / 2);

	printf("\n\nEntering main loop\n\n");
	while(1) {
		check_cmd();
	}

	return 0;
}
