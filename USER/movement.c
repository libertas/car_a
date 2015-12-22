#include "clock.h"
#include "encoder.h"
#include "movement.h"


uint32_t speeds[3] = {0};

uint32_t get_speed(uint8_t wheel)
{
	return speeds[wheel];
}

void move_y(uint32_t y)
{
	#ifdef USE_THREE_WHEEL
	t_move_y(y);
	#endif
}

void move_up(void)
{
	uprintf(USART1, "3V1000\r4V1000\r");
	delay_ms(10);
}

void stop_move_up(void)
{
	uprintf(USART1, "3V0\r4V0\r");
	delay_ms(10);
}
