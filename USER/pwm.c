#include <stdio.h>

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "pwm.h"

uint32_t PWMHighTime = 0;
uint32_t PWMTotal = 0;

inline void timer2_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
  TIM_TimeBaseInitStructure.TIM_Period = 9;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PWM_PRESCALE;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
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
	set_freq(56);
	set_duty(0.08);
	printf("%lf\n", (double)PWMHighTime / PWMTotal);

	timer2_config();
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
}
