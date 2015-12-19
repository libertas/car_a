#include "movement.h"


// (x, y)
double position[2] = {0};

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
