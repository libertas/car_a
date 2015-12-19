#include "movement.h"

void move_y(uint32_t y)
{
	#ifdef USE_THREE_WHEEL
	t_move_y(y);
	#endif
}
