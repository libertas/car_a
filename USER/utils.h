#include <stdint.h>


#ifndef UTILS_H
#define UTILS_H

/*
	To avoid the bug of the compilers
*/
double int2double(signed int i);

#define ABS(x) ((x) >= 0? (x): (-1) * (x))

typedef struct
{
	char *data;
	uint16_t front, rear;
	uint16_t count, max_size;
}	char_queue;

int init_char_queue(char_queue *p, char buf[], uint16_t max_size);
int in_char_queue(char_queue *p, char c);
int out_char_queue(char_queue *p, char *c);

#endif
