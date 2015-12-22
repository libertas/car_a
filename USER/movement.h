#include <math.h>
#include <stdint.h>

#include "usart.h"

/*
The world view:
	front = +y
	back = -y
	right = +x
	left = -x
	up = +z
	down = -z
*/

#define ZERO (1E-3)
#define PI 3.141592653589793

#define W_DIAMETER 120  // millimeter
#define USE_THREE_WHEEL

#ifdef USE_THREE_WHEEL


#define VECT_W1 (+1)  // the wheel on the left or right side
#define VECT_W2 (+1)  // the front wheel
#define VECT_W3 (+1)  // the back wheel

void t_move_y(uint32_t y);

#endif

extern uint32_t speeds[3];


void move_up(void);
void move_y(uint32_t y);

void stop(void);
void stop_all(void);
void stop_move_up(void);

