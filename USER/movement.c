#include "clock.h"
#include "debug.h"
#include "encoder.h"
#include "fan.h"
#include "movement.h"


int16_t get_speed(uint8_t wheel)
{
	return arg_speeds[wheel];
}

void stop_all(void)
{
	uprintf(USART1, "V0\r");
	delay_ms(10);
}

void move_x(float x)
{
	#ifdef DEBUG
	printf("\nmove_x(%f)\n", x);
	#endif
	
	#ifdef USE_THREE_WHEEL
	t_move_x(x);
	#endif
}

void move_x_c(uint8_t dir)
{
	#ifdef USE_THREE_WHEEL
	t_move_x_c(dir);
	#endif
}

void move_y(float y)
{
	#ifdef DEBUG
	printf("\nmove_y(%f)\n", y);
	#endif

	#ifdef USE_THREE_WHEEL
	t_move_y(y);
	#endif
}

void move_y_c(uint8_t dir)
{
	#ifdef USE_THREE_WHEEL
	t_move_y_c(dir);
	#endif
}

void stop(void)
{
	#ifdef USE_THREE_WHEEL
	t_stop();
	#endif
	delay_ms(10);
}

void move_up(void)
{
	uprintf(USART1, "4V-8000\r");
	delay_ms(1);
	uprintf(USART1, "3V8000\r");
	delay_ms(1);
	
	start_fan();
}

void stop_move_up(void)
{
	uprintf(USART1, "4V0\r");
	delay_ms(1);
	uprintf(USART1, "3V0\r");
	delay_ms(1);
	
	stop_fan();
}
