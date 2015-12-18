#include <stdio.h>

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "pwm.h"

uint32_t PWMHighTime = 0;
uint32_t PWMTotal = 0;

inline void timer9_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	
  TIM_TimeBaseInitStructure.TIM_Period = 19;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PWM_PRESCALE;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM9, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void pwm_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	PWMTotal = 10000;
	PWMHighTime = 5000;
	set_freq(50);
	set_duty(0.077);

	timer9_config();
}

void set_duty(double duty)
{
	if(duty >= 0 && duty <= 1)
	{
		PWMHighTime = PWMTotal * duty;
	}
	else
	{
		printf("Wrong Duty!\n");
	}
}

void set_freq(unsigned long freq)
{
	double duty;
	duty = (double) PWMHighTime / PWMTotal;
	PWMTotal = PWM_FREQ / freq;
	PWMHighTime = PWMTotal * duty;
	printf("%ld\n", PWMTotal);
}
