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
#include "switch.h"
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

void start_1(void)
{
	XY_DEFAULT_SPD = 800;
	ROTATE_DEFAULT_SPD = 600;
	auto_continous_flag = true;
}

void start_2(void)
{
	XY_DEFAULT_SPD = 1500;
	
	#ifdef USE_LEFT_FIELD
	ROTATE_DEFAULT_SPD = 2000;
	#endif
	
	#ifdef USE_RIGHT_FIELD
	ROTATE_DEFAULT_SPD = 1000;
	#endif
	
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
	
	#ifdef USE_LEFT_FIELD
	ROTATE_DEFAULT_SPD = 300;
	#endif
	
	#ifdef USE_RIGHT_FIELD
	ROTATE_DEFAULT_SPD = 2000;
	#endif
	
	set_duty(FAN_CHANNEL, 0.06f);
}

void fan_up_3(void)
{
	fan_up_auto(0.15f);
	XY_DEFAULT_SPD = 1000;
	ROTATE_DEFAULT_SPD = 300;
}

void roll_fan_1(void)
{
	fan_roll_r(1);
	
	auto_continous_flag = false;
	for(uint8_t i = 0; i < 20; i++) {
		set_duty(FAN_CHANNEL, 0.055f + i * 0.001f);
		delay_ms(100);
	}
	delay_ms(1000);
	stop_fan();
	XY_DEFAULT_SPD = 1000;
	
	#ifdef USE_LEFT_FIELD
	ROTATE_DEFAULT_SPD = 300;
	#endif
	
	#ifdef USE_RIGHT_FIELD
	ROTATE_DEFAULT_SPD = 1000;
	#endif
	
	auto_continous_flag = true;
}

void adjust_0(void)
{
	#ifdef USE_LEFT_FIELD
	ROTATE_DEFAULT_SPD = 500;
	#endif
	
	#ifdef USE_RIGHT_FIELD
	ROTATE_DEFAULT_SPD = 500;
	#endif
	
	XY_DEFAULT_SPD = 2500;
}

void adjust_1(void)
{
	fan_up_flag = 0;
	fan_des = 0;
	fan_down(10);
	
	#ifdef USE_LEFT_FIELD
	ROTATE_DEFAULT_SPD = 2500;
	#endif
	
	#ifdef USE_RIGHT_FIELD
	ROTATE_DEFAULT_SPD = 400;
	#endif
	
	XY_DEFAULT_SPD = 2500;
}

void adjust_2(void)
{
	#ifdef USE_LEFT_FIELD
	ROTATE_DEFAULT_SPD = 2000;
	#endif
	
	#ifdef USE_RIGHT_FIELD
	ROTATE_DEFAULT_SPD = 400;
	#endif
	
	XY_DEFAULT_SPD = 2500;
	fan_up_auto(0.1f);
}

void adjust_3(void)
{
	auto_continous_flag = false;
	
	#ifdef USE_LEFT_FIELD
	ROTATE_DEFAULT_SPD = 1000;
	#endif
	
	#ifdef USE_RIGHT_FIELD
	ROTATE_DEFAULT_SPD = 1000;
	#endif
	
	XY_DEFAULT_SPD = 2500;
}

#ifdef USE_LEFT_FIELD
struct coordinate_t coord[] = {
	{2600, 0, 0, start_1},\
	{2600, 900, 0, start_2},\
	{2500, 1072.85, PI/12, fan_up_1},\
	{2500, 2133.29, PI/6},\
	{1400, 2860.83, PI/4, fan_up_2},\
	{300, 3647.91, PI/6},\
	{200, 5090.82, PI/12, fan_up_3},\
	{200, 6700, 0, roll_fan_1},\
	{200, 8000, 0, adjust_0},\
	{200, 10500, 0, adjust_1},\
	{1400, 12000, -PI * 2 / 5, adjust_2},\
	{2000, 12600, -PI/2, adjust_3},\
	{4800, 13100, -PI/2},\
	{0, 0, 0, 0}
};
#endif

#ifdef USE_RIGHT_FIELD
struct coordinate_t coord[] = {
	{-2600, 0, 0, start_1},\
	{-2600, 900, 0, start_2},\
	{-2500, 1072.85, -PI/12, fan_up_1},\
	{-2500, 2133.29, -PI/6},\
	{-1400, 2860.83, -PI/4, fan_up_2},\
	{-800, 3647.91, -PI/6},\
	{-600, 5090.82, -PI/12, fan_up_3},\
	{-300, 6700, 0, roll_fan_1},\
	{-600, 7000, PI / 12, adjust_0},\
	{-600, 8000, PI / 6, adjust_1},\
	{-1400, 12000, PI * 2 / 5, adjust_2},\
	{-2000, 12600, PI/2, adjust_3},\
	{-4700, 13100, PI/2},\
	{0, 0, 0, 0}
};
#endif

void auto_start(void)
{
	push_rod_c(1, 3);
	automove_config();
	automove_flag = true;

	printf("\nAuto controlling.\n");
	auto_flag = true;
	
	start_fan();

	for(int i = 0; 0 != coord[i].x || 0 != coord[i].y || 0 != coord[i].rad || 0 != coord[i].callback; i++) {
		set_auto_dest(coord[i].x / 1000, coord[i].y / 1000, coord[i].rad);
		while(!near_auto_dest() && automove_flag);
		if(0 != coord[i].callback && automove_flag)
			(coord[i].callback)();
	}
	
	automove_flag = false;
	EXTI_ClearITPendingBit(EXTI_Line3);
	switch_config(2);
	switch_nvic_enable(2);
	wl_run();
	printf("out\n");
	stop();
	
	push_rod_c(PUSH_ROD_PUSH, 1);
	push_rod_c(PUSH_ROD_PUSH, 2);
	stop();
	
	while(1) {
		printf("moving up\n");
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
