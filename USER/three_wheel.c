#include "debug.h"
#include "encoder.h"
#include "movement.h"

#ifdef USE_THREE_WHEEL

void t_move_y(float y)
{
	float pos_y;
	float old_pos[2];

	old_pos[0] = get_pos_x();
	old_pos[1] = get_pos_y();
	
	#ifdef DEBUG
	printf("old_pos[1]:%f\n", old_pos[1]);
	#endif

	speeds[0] = VECT_W1 * 10000;
	speeds[1] = VECT_W2 * speeds[0] * cos(PI / 2);
	speeds[1] = VECT_W3 * speeds[0] * cos(PI / 2);

	uprintf(USART1,\
		"1V%d\r2V%d\r3V%d\r",\
		(uint16_t) speeds[0],\
		(uint16_t) speeds[1],\
		(uint16_t) speeds[2]\
		);
	
	do {
		pos_y = get_pos_y();

		#ifdef DEBUG
		printf("pos_y:%f\n", pos_y);
		#endif

	} while(pos_y - y - old_pos[1] < ZERO);
	
	speeds[0] = 0;
	speeds[1] = 0;
	speeds[2] = 0;
	
	uprintf(USART1,\
		"1V%d\r2V%d\r3V%d\r",\
		(uint16_t) speeds[0],\
		(uint16_t) speeds[1],\
		(uint16_t) speeds[2]\
		);
}

#endif
