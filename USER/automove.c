#include <math.h>

#include <misc.h>
#include <stm32f4xx_tim.h>

#include "automove.h"
#include "encoder.h"
#include "movement.h"

#include "mti.h"

#ifdef USE_FOUR_WHEEL

void auto_clr_spd(void)
{
	for(uint8_t i = 0; i < 4; i++) {
		arg_speeds[i] = 0;
	}
}

void auto_rotate(float now_rad, float dest_rad)
{
}

void auto_move_xy(float x, float y, float dest_x, float dest_y)
{
}

void auto_send(void)
{
	uprintf(USART1,\
		"\r0V%d\r1V%d\r2V%d\r5V%d\r",\
		arg_speeds[0],\
		arg_speeds[1],\
		arg_speeds[2],\
		arg_speeds[3]\
		);
}

#endif

float gps_dest_x = 0, gps_dest_y = 0, gps_dest_rad = 0;
float gps_x = 0, gps_y = 0, gps_rad = 0;

void automove_daemon(void)
{
	float x, y, rad;
	float tmp;

	rad = get_mti_value();
	
	tmp = rad * (EX_X * EX_X + EX_Y * EX_Y) / EX_Y;
	x = get_pos_x() - tmp;

	tmp = rad * (EY_X * EY_X + EY_Y * EY_Y) / EY_Y;
	y = get_pos_y() - tmp;
	
	gps_x = x;
	gps_y = y;
	gps_rad = rad;

	//auto_clr_spd();
	auto_rotate(gps_rad, gps_dest_rad);
	auto_move_xy(gps_x, gps_y, gps_dest_x, gps_dest_y);
	//auto_send();
}

float get_gps_x(void)
{
	return gps_x;
}

float get_gps_y(void)
{
	return gps_y;
}

float get_gps_rad(void)
{
	return gps_rad;
}

void tim10_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = AUTOMOVE_DAEMON_TIME * 1000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 168 - 1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM10, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void automove_config(void)
{
	tim10_config();
}
