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

#define ZERO ((float) (1E-1))
#define PI 3.141592653589793

#define W_DIAMETER ((float) 0.12)  // meter
#define USE_THREE_WHEEL

#ifdef USE_THREE_WHEEL

#define DEFAULT_ARG_SPEED 2000
#define VECT_W0 (-1)  // the wheel on the left or right side
#define VECT_W1 (+1)  // the front wheel
#define VECT_W2 (+1)  // the back wheel

extern int16_t arg_speeds[3];

void t_move_x(float x);
void t_move_y(float y);
void t_stop(void);

#endif


void move_up(void);
void move_x(float x);
void move_y(float y);

void stop(void);
void stop_all(void);
void stop_move_up(void);

