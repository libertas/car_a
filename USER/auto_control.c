#include <stdint.h>

#include "auto_control.h"

void start(void)
{
}

void (*auto_steps[])(void) = {start, 0};

void auto_control(void)
{
	for(uint16_t i = 0; auto_steps[i] != 0; i++) {
		auto_steps[i]();
	}
}
