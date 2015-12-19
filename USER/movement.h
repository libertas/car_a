#include <stdint.h>

/*
The world view:
	front = +y
	back = -y
	right = +x
	left = -x
	up = +z
	down = -z
*/

#define USE_THREE_WHEEL

#ifdef USE_THREE_WHEEL

#include "three_wheel.h"

#endif

void move_y(uint32_t y);
