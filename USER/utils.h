#include <stdint.h>

/*
	To avoid the bug of the compilers
*/
double int2double(signed int i);

#define ABS(x) ((x) >= 0? (x): (-1) * (x))
