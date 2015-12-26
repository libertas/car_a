#include "utils.h"

double int2double(signed int i)
{
	double result;
	if(i > 32767) {
		result =  (double)((unsigned int) i) - 65536.0;
	} else {
		result = (double) i;
	}
	return result;
}
