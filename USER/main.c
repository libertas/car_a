#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "usart.h"

void tmp_config(void);

int main(void)
{
	systick_config();
	usart1_config(115200);
	
	while(1)
	{
	}

	return 0;
}

