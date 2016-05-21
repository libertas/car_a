#include <stdint.h>

#include "automove.h"
#include "auto_control.h"
#include "clock.h"
#include "fan.h"
#include "interpreter.h"
#include "magnet.h"
#include "movement.h"
#include "mti.h"
#include "push_rod.h"
#include "pwm.h"
#include "whiteline.h"

bool auto_flag = false;
float old_pos_x = 0, old_pos_y = 0, old_rad = 0;
float dest_pos_x = 0, dest_pos_y = 0, dest_rad = 0;

struct coordinate_t {
	float x;
	float y;
	float rad;
	void (*callback)(void);
};

void start_fan_1(void)
{
	start_fan();
	
	delay_ms(2000);

	automove_flag = false;
	wl_run();

	set_auto_dest(get_gps_x(), get_gps_y(), -PI / 2);
	stop();
	automove_flag = true;

	delay_ms(2000);
	automove_flag = false;
	stop();

	push_rod_c(PUSH_ROD_PUSH, 1);
	push_rod_c(PUSH_ROD_PUSH, 2);

	while(move_up_flag) {
		move_up();
		delay_ms(10);
	}
}

struct coordinate_t coord[] = {
	{2812.9, 0, 0, start_fan_1},\
	{0, 0, 0, 0}
};

void auto_start(void)
{
	automove_config();
	automove_flag = true;

	printf("\nAuto controlling.\n");
	auto_flag = true;

	for(int i = 0; 0 != coord[i].x || 0 != coord[i].y || 0 != coord[i].rad || 0 != coord[i].callback; i++) {
		set_auto_dest(coord[i].x / 1000, coord[i].y / 1000, coord[i].rad);
		while(!near_auto_dest());
		if(0 != coord[i].callback)
			(coord[i].callback)();
	}
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
