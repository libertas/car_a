#include <stdint.h>

#include "stm32f4xx_tim.h"
#include "push_rod.h"


void push_rod_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOE, GPIO_Pin_0 | GPIO_Pin_1, Bit_RESET);


	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1680 - 1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);


	NVIC_InitStructure.NVIC_IRQChannel = TIM8_TRG_COM_TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void push_rod_c(uint8_t dir)
{
	switch(dir){
		case 0:{
			GPIO_WriteBit(GPIOE, GPIO_Pin_0, Bit_SET);
			GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET);
			break;
		}
		case 1:{
			GPIO_WriteBit(GPIOE, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET);
			break;
		}
		case 0xff:{
			GPIO_WriteBit(GPIOE, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET);
			break;
		}
		default:break;
	}
}
void push_rod(uint8_t dir)
{
	switch(dir){
		case 0:{
			push_rod_c(PUSH_ROD_PUSH);
			TIM_Cmd(TIM14, ENABLE);
			break;
		}
		case 1:{
			push_rod_c(PUSH_ROD_PULL);
			TIM_Cmd(TIM14, ENABLE);
			break;
		}
		case 0xff:{
			push_rod_c(PUSH_ROD_STOP);
			TIM_Cmd(TIM14, ENABLE);
			break;
		}
		default:break;
	}
}
