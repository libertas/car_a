#include <stdint.h>

#include "auto_control.h"
#include "clock.h"
#include "fan.h"
#include "interpreter.h"

void auto_start(void)
{
	delay_ms(2000);
	start_fan();
}

void auto_stop(void)
{
	check_cmd();
}

void (*auto_steps[])(void) = {auto_start, auto_stop, 0};

void auto_control(void)
{
	for(uint16_t i = 0; auto_steps[i] != 0; i++) {
		auto_steps[i]();
	}
}
