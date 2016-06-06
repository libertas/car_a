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
bool manual_auto_flag = false;
float old_pos_x = 0, old_pos_y = 0, old_rad = 0;
float dest_pos_x = 0, dest_pos_y = 0, dest_rad = 0;

struct coordinate_t {
	float x;
	float y;
	float rad;
	void (*callback)(void);
};


void enter_pole(void)
{
	//fan_up_auto(0.55f);
	stop_all();
	push_rod_c(PUSH_ROD_PUSH, 2);//push_rod
	delay_ms(1000);
	while(move_up_flag) {
		move_up();
		delay_ms(10);
	}
}

void manual_to_auto(void)
{
	manual_auto_flag = true;
}

struct coordinate_t coord[] = {
	{100, 5475, 0, enter_pole},\
	{0, 0, 0, 0}
};

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
	TIM_Cmd(TIM10, DISABLE);

	auto_flag = false;
	
	printf("\nAuto control stopped.\n");
	while(1){
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
