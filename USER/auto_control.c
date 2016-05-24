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
	XY_DEFAULT_SPD = 2000;
	ROTATE_DEFAULT_SPD = 2000;
	auto_continous_flag = true;
}
void fan_up_1(void)
{
	fan_up_auto(0.2f);
}

void fan_up_2(void)
{
	fan_up_auto(0.2f);
	XY_DEFAULT_SPD = 2000;
	ROTATE_DEFAULT_SPD = 500;
}

void fan_up_3(void)
{
	fan_up_auto(0.15f);
	XY_DEFAULT_SPD = 1000;
	ROTATE_DEFAULT_SPD = 250;
}

void roll_fan_1(void)
{
	auto_continous_flag = false;
	delay_ms(3000);
	stop_fan();
	XY_DEFAULT_SPD = 2000;
	ROTATE_DEFAULT_SPD = 500;
	auto_continous_flag = true;
}

void fan_down_1(void)
{
	fan_down(10);
}

void adjust_1(void)
{
	ROTATE_DEFAULT_SPD = 500;
	XY_DEFAULT_SPD = 2500;
}

void adjust_2(void)
{
	ROTATE_DEFAULT_SPD = 500;
	XY_DEFAULT_SPD = 2500;
}

void adjust_3(void)
{
	auto_continous_flag = false;
	ROTATE_DEFAULT_SPD = 750;
	XY_DEFAULT_SPD = 1500;
}

struct coordinate_t coord[] = {
	{2900, 0, 0, start_fan_1},\
	{2900, 1072.85, PI/12, fan_up_1},\
	{2900, 2133.29, PI/6},\
	{2300, 2860.83, PI/4, fan_up_2},\
	{1100, 3647.91, PI/6},\
	{450, 5090.82, PI/12, fan_up_3},\
	{200, 6700, 0, roll_fan_1},\
	{200, 9000, 0, adjust_1},\
	{2500, 12400, -PI * 2 / 5, adjust_2},\
	{2600, 12600, -PI/2, adjust_3},\
	{5000, 13100, -PI/2},\
	{0, 0, 0, 0}
};

void auto_start(void)
{
	automove_config();

	printf("\nAuto controlling.\n");
	auto_flag = true;

	for(int i = 0; 0 != coord[i].x || 0 != coord[i].y || 0 != coord[i].rad || 0 != coord[i].callback; i++) {
		set_auto_dest(coord[i].x / 1000, coord[i].y / 1000, coord[i].rad);
		while(!near_auto_dest() && automove_flag);
		if(0 != coord[i].callback && automove_flag)
			(coord[i].callback)();
	}
	
	automove_flag = false;
	set_auto_dest(get_gps_x() + 0.1f, get_gps_y(), -PI / 2);
	ROTATE_DEFAULT_SPD = 2500;
	XY_DEFAULT_SPD = 2500;
	auto_continous_flag = false;
	automove_flag = true;
	delay_ms(2000);
	automove_flag = false;
	stop();
	
	push_rod(PUSH_ROD_PUSH, 0);
	push_rod(PUSH_ROD_PUSH, 1);
	stop();
	
	delay_ms(500);
	
	stop();
	
	while(1) {
		move_up();
		delay_ms(100);
	}
}

void auto_stop(void)
{
	automove_flag = false;
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
