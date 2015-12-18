#include <stdio.h>

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "pwm.h"

uint32_t PWMHighTime[PWM_CHANNEL_NUM];
uint32_t PWMTotal[PWM_CHANNEL_NUM];

uint16_t PWMPins[PWM_CHANNEL_NUM] = {GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5};
GPIO_TypeDef *PWMPorts[PWM_CHANNEL_NUM] = {GPIOA, GPIOA, GPIOA, GPIOA, GPIOA};

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
	double duties[PWM_CHANNEL_NUM] = {0.08, 0.09, 0.8, 0.9, 0.5};
	unsigned long freqs[PWM_CHANNEL_NUM] = {56, 50, 20, 8, 100};
	
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	for(i = 0; i < PWM_CHANNEL_NUM; i++) {
		PWMTotal[i] = 10000;
		PWMHighTime[i] = 5000;
	}
	
	set_duty(duties);
	set_freq(freqs);

	timer9_config();
}

void set_duty(double duty[PWM_CHANNEL_NUM])
{
	uint8_t i;
	for(i = 0; i < PWM_CHANNEL_NUM; i++) {
		if(duty[i] >= 0 && duty[i] <= 1)
		{
			PWMHighTime[i] = PWMTotal[i] * duty[i];
		}
		else
		{
			printf("Wrong Duty!\n");
		}
	}
}

void set_freq(unsigned long freq[PWM_CHANNEL_NUM])
{
	double duty;
	uint8_t i;
	for(i = 0; i < PWM_CHANNEL_NUM; i++) {
		duty = (double) PWMHighTime[i] / PWMTotal[i];
		PWMTotal[i] = PWM_FREQ / freq[i];
		PWMHighTime[i] = PWMTotal[i] * duty;
		printf("%ld\n", PWMTotal[i]);
	}
}

void set_all_duty(double d)
{
}

void set_all_freq(unsigned long f)
{
}
