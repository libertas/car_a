#include <stdio.h>

#include "stm32f4xx.h"

#include "usart.h"

int main(void)
{
	usart1_config(115200);
	
	while(1)
	{
	}
	
	return 0;
}
