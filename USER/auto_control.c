#include <stdint.h>

#include "auto_control.h"
#include "clock.h"
#include "fan.h"
#include "interpreter.h"
#include "movement.h"
#include "mti.h"

bool auto_flag = false;

void auto_start(void)
{
	printf("\nAuto controlling.\n");
	auto_flag = true;
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

void step1(void)
{
	uprintf(USART1, "\rAC15\r");
	move_xy(3, 0);

	uprintf(USART1, "\rAC15\r");
	delay_ms(500);
	f_rotate(0 - get_mti_value());
	delay_ms(500);
}

void step2(void)
{
	start_fan();

	uprintf(USART1, "\rAC10000\r");

	move_arc(1.2f, PI * 0.20f);

	move_arc(1.8f, -PI * 0.15f);

	stop_fan();
	
	uprintf(USART1, "\rAC15\r");
	delay_ms(500);
	f_rotate(0 - get_mti_value());
	delay_ms(500);
}

void step3(void)
{
	uprintf(USART1, "\rAC15\r");
	move_xy(0, 2.5);
	delay_ms(500);
}

void step4(void)
{
	uprintf(USART1, "\rAC10000\r");
	move_arc(1.5f, -PI / 2);
}

void step5(void)
{
	uprintf(USART1, "\rAC15\r");
	delay_ms(500);
	rotate(-PI - get_mti_value());
	delay_ms(500);
	
	move_xy(-2, 0);
}

void tim10_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void (*auto_steps[])(void) = {auto_start, step1, step2, step3, step4, step5, auto_stop, 0};

void auto_control(void)
{
	tim10_config();
	tim14_config();

	for(uint16_t i = 0; auto_steps[i] != 0; i++) {
		auto_steps[i]();
	}
}
