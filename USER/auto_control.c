#include <stdint.h>

#include "auto_control.h"
#include "clock.h"
#include "fan.h"
#include "interpreter.h"
#include "movement.h"

bool auto_flag = false;

void auto_start(void)
{
	auto_flag = true;
	delay_ms(2000);
	start_fan();
}

void auto_stop(void)
{
	stop_all();
	stop_fan();

	auto_flag = false;
	
	printf("\nAuto control stopped.\n");
	while(1) {
		check_cmd();
	}
}

void (*auto_steps[])(void) = {auto_start, auto_stop, 0};

void auto_control(void)
{
	for(uint16_t i = 0; auto_steps[i] != 0; i++) {
		auto_steps[i]();
	}
}
