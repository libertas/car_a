#include "climb.h"
#include "usart.h"
//#include "cmd.h"

int climb_v = 0;

void climb_up(void)
{
		uprintf(USART1, "\r3V-%d\r4V-%d\r", climb_v,climb_v);
}


void climb_down(void)
{
		uprintf(USART1, "\r3V%d\r4V%d\r", climb_v, climb_v);
}

void climb_stop(void)
{
		uprintf(USART1, "\r3V0\r4V0\r");
}
