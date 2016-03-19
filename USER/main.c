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

	char str[10] = "012345678";
	char str1[10] = "87654321";
	char tmp[1000];
	db_clear_init();
	db_save("testa", (uint8_t*)str, 5);
	db_save("testb", (uint8_t*)str, 5);
	db_save("testc", (uint8_t*)str, 5);
	db_sync();
	uint32_t d = db_find("testa");
	printf("%d\n", d);
	d = db_find("testb");
	printf("%d\n", d);
	d = db_find("testc");
	printf("%d\n", d);
	db_read("testc", (uint8_t*)tmp);
	printf("%s\n", tmp);
	flreadn(DB_SECTOR, (uint32_t*)tmp, 30);
	printf("%s\n", tmp);
	
	db_delete("testb", 5);
	db_save("testc", (uint8_t*)str1, 5);
	db_sync();
	
	flreadn(DB_SECTOR, (uint32_t*)tmp, 30);
	printf("\n%s\n", tmp);


	printf("\n\nEntering main loop\n\n");
	while(1)
	{
		check_cmd();
	}

	return 0;
}
