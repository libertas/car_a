#include <stdio.h>

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "pwm.h"

#ifndef USE_HPWM

uint32_t PWMHighTime[PWM_CHANNEL_NUM];
uint32_t PWMTotal[PWM_CHANNEL_NUM];

uint16_t PWMPins[PWM_CHANNEL_NUM] = {GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11,\
	GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9};
GPIO_TypeDef *PWMPorts[PWM_CHANNEL_NUM] = {GPIOA, GPIOA, GPIOA, GPIOA, GPIOC, GPIOC, GPIOC, GPIOC};


void rcc_io_config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
}

void pwm_config(void)
{
	double duties[PWM_CHANNEL_NUM] = {0.06, 0.06, 0.076, 0.076, 0.076, 0.076, 0.076, 0.076};
	unsigned long freqs[PWM_CHANNEL_NUM] = {50, 50, 50, 50, 50, 50, 50, 50};
	
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	rcc_io_config();

	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	for(i = 0; i < PWM_CHANNEL_NUM; i++) {
		GPIO_InitStructure.GPIO_Pin = PWMPins[i];
		GPIO_Init(PWMPorts[i], &GPIO_InitStructure);
	}


	for(i = 0; i < PWM_CHANNEL_NUM; i++) {
		PWMTotal[i] = 10000;
		PWMHighTime[i] = 5000;
	}
	
	set_duties(duties);
	set_freqs(freqs);


	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
  TIM_TimeBaseInitStructure.TIM_Period = 19;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PWM_PRESCALE;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void set_duty(uint8_t channel, double duty)
{
	PWMHighTime[channel] = PWMTotal[channel] * duty;
}

void set_freq(uint8_t channel, unsigned long freq)
{
	double duty;

	duty = (double) PWMHighTime[channel] / PWMTotal[channel];
	PWMTotal[channel] = PWM_FREQ / freq;
	PWMHighTime[channel] = PWMTotal[channel] * duty;
}

void set_duties(double duties[PWM_CHANNEL_NUM])
{
	uint8_t i;
	for(i = 0; i < PWM_CHANNEL_NUM; i++) {
		if(duties[i] >= 0 && duties[i] <= 1)
		{
			set_duty(i, duties[i]);
		}
		else
		{
			printf("Wrong Duty!\n");
		}
	}
}

void set_freqs(unsigned long freqs[PWM_CHANNEL_NUM])
{
	uint8_t i;
	for(i = 0; i < PWM_CHANNEL_NUM; i++) {
		set_freq(i, freqs[i]);
	}
}

#endif
