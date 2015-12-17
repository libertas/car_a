#include <stdio.h>

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "pwm.h"

uint8_t PWMState;
uint32_t PWMHighTime;
uint32_t PWMLowTime;
uint32_t PWMTotal;

void timer2_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = PWM_PRESCALE;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// Set up timer2 interrupt
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
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

	PWMState = 0;
	PWMTotal = 10000;
	PWMHighTime = 5000;
	PWMLowTime = 5000;
	set_freq(56);
	set_duty(0.08);
	
	timer2_config();
}

void set_duty(double duty)
{
	if(duty >= 0 && duty <= 1)
	{
		PWMHighTime = PWMTotal * duty;
		PWMLowTime = PWMTotal - PWMHighTime;
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
	PWMLowTime = PWMTotal - PWMHighTime;
}
