#include "clock.h"
#include "debug.h"
#include "encoder.h"
#include "movement.h"

#ifdef USE_THREE_WHEEL

void t_move_x(float x)
{
	float pos_x;
	float old_pos[2];

	old_pos[0] = get_pos_x();
	old_pos[1] = get_pos_y();
	
	arg_speeds[0] = 0;
	arg_speeds[1] = VECT_W1 * DEFAULT_ARG_SPEED * 2 / sqrt(3);
	arg_speeds[2] = 0 - VECT_W2 * arg_speeds[1];
	
	#ifdef DEBUG
	printf("arg_speeds:\n");
	printf("\t0:%d\n", arg_speeds[0]);
	printf("\t1:%d\n", arg_speeds[1]);
	printf("\t2:%d\n", arg_speeds[2]);
	delay_ms(3000);
	#endif

	uprintf(USART1,\
		"\r0V%d\r1V%d\r2V%d\r",\
		arg_speeds[0],\
		arg_speeds[1],\
		arg_speeds[2]\
		);
	
	do {
		pos_x = get_pos_x();

		#ifdef DEBUG
		printf("pos_x:%f\n", pos_x);
		#endif

	} while(pos_x - x - old_pos[0] < ZERO);
	
	arg_speeds[0] = 0;
	arg_speeds[1] = 0;
	arg_speeds[2] = 0;
	
	uprintf(USART1,\
		"\r0V%d\r1V%d\r2V%d\r",\
		arg_speeds[0],\
		arg_speeds[1],\
		arg_speeds[2]\
		);
}

void t_move_y(float y)
{
	float pos_y;
	float old_pos[2];

	old_pos[0] = get_pos_x();
	old_pos[1] = get_pos_y();

	arg_speeds[0] = DEFAULT_ARG_SPEED;
	arg_speeds[1] = arg_speeds[0] / cos(PI / 6);
	arg_speeds[2] = arg_speeds[1];
	arg_speeds[0] *= VECT_W0;
	arg_speeds[1] *= VECT_W1;
	arg_speeds[2] *= VECT_W2;
	
	#ifdef DEBUG
	printf("arg_speeds:\n");
	printf("\t0:%d\n", arg_speeds[0]);
	printf("\t1:%d\n", arg_speeds[1]);
	printf("\t2:%d\n", arg_speeds[2]);
	delay_ms(3000);
	#endif

	uprintf(USART1,\
		"\r0V%d\r1V%d\r2V%d\r",\
		arg_speeds[0],\
		arg_speeds[1],\
		arg_speeds[2]\
		);
	
	do {
		pos_y = get_pos_y();

		#ifdef DEBUG
		printf("pos_y:%f\n", pos_y);
		#endif

	} while(pos_y - y - old_pos[1] < ZERO);
	
	arg_speeds[0] = 0;
	arg_speeds[1] = 0;
	arg_speeds[2] = 0;
	
	uprintf(USART1,\
		"\r0V%d\r1V%d\r2V%d\r",\
		arg_speeds[0],\
		arg_speeds[1],\
		arg_speeds[2]\
		);
}

#endif
