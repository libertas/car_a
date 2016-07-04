#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "usart.h"

int main(void)
{
	system_clk_set();
	systick_config();
	
	uart_config(115200);
	
	uprintf(UART5,"BUPT");
	while(1)
	{
	}
	
	return 0;
}
