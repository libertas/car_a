#include "maxon.h"
#include "usart.h"

void maxon_config(void)
{
		uprintf(USART1, "s r0x24 11\r\n\n");
		uprintf(USART1, "s r0x36 200\r\n\n");
		uprintf(USART1, "s r0x37 400\r\n\n");		
}

void maxon_up(int maxon_v)
{
		uprintf(USART1, "s r0x2f %d\r\n\n", maxon_v);
}

void maxon_down(int maxon_v)
{
		uprintf(USART1, "s r0x2f -%d\r\n\n", maxon_v);
}

void maxon_stop(void)
{
		uprintf(USART1, "s r0x2f 0\r\n\n");
}

void maxon_current(int rate, int out)
{
		uprintf(USART1, "s r0x6a %d\r\n\n", rate);
		uprintf(USART1, "s r0x02 %d\r\n\n", out);
}
