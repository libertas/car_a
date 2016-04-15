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

bool auto_flag = false;
float old_pos_x = 0, old_pos_y = 0, old_rad = 0;
float dest_pos_x = 0, dest_pos_y = 0, dest_rad = 0;

float a[][3] = {\
	{2812.9, 0, 0 },\
	{2812.9, 1072.85, PI/12},\
	{2550.44, 2133.29, PI/6},\
	{2130.4, 2860.83, PI/4},\
	{1361.84, 3647.91, PI/6},\
	{800, 5090.82, PI/12},\
	{700, 6734.66, 0},\
	{700, 9500, 0},\
	{1500, 10500, PI},\
	{4960, 11900, PI},\
	{5900, 11900, PI - 0.2f},\
	{6500, 11900, PI - 0.4f},\
	{7100, 11900, PI - 0.6f},\
	{0}
};

void auto_start(void)
{
	automove_config();

	printf("\nAuto controlling.\n");
	auto_flag = true;

	for(int i = 0; 0 != a[i][0]; i++) {
		set_auto_dest(a[i][0] / 1000, a[i][1] / 1000, a[i][2]);
		while(!near_auto_dest());
	}
}

void auto_up(void)
{
	push_rod(PUSH_ROD_PUSH, 0);
	move_up();
	while(0 == GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9)) {
		
	}
	stop_all();
	delay_ms(1000);
	mag_in();//��������
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

void tim14_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 168 - 1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM14, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_TRG_COM_TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void (*auto_steps[])(void) = {auto_start, auto_up, auto_stop, 0};

void auto_control(void)
{
	tim14_config();

	for(uint16_t i = 0; auto_steps[i] != 0; i++) {
		auto_steps[i]();
	}
}
