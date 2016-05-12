#include "climb.h"
#include "usart.h"
#include "parameter.h"

int climb_v = 0;
param_struct *climb_param;

void climb_up(void)
{
		climb_v = climb_param -> climb_v;
		uprintf(USART1, "\r3V%d\r4V%d\r", climb_v);
}

void climb_config(void)
{
		param_init(&climb_param);
}

void climb_down(void)
{
		climb_v = climb_param -> climb_v;
		uprintf(USART1, "\r3V-%d\r4V-%d\r", climb_v);
}

void climb_stop(void)
{
		uprintf(USART1, "\r3V0\r4V0\r");
}
