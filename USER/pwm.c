#include <stdio.h>

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "car.h"
#include "pwm.h"

#ifndef USE_HPWM

uint32_t PWMHighTime[PWM_CHANNEL_NUM];
uint32_t PWMTotal[PWM_CHANNEL_NUM];
/*
	0 fan 涵道风扇
	1 fan_updown 无刷
	2 fan_roll 舵机 0.6 - 0.12, 100Hz
	3 mag_in/out 舵机	0.045(start) - 0.103(end)
	4 mag_near/far 舵机  0.05(end) - 0.09(start)
	*无刷0.71stop,0.76/0.65正/反转
	*舵机0~0.12
*/

#ifdef CAR_A_1
uint16_t PWMPins[PWM_CHANNEL_NUM] = {\
	GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_9,\
	GPIO_Pin_10, GPIO_Pin_8};
GPIO_TypeDef *PWMPorts[PWM_CHANNEL_NUM] = {\
	GPIOG, GPIOG, GPIOC,\
	GPIOA, GPIOC};
#endif

#ifdef CAR_A_2
/*
	0 fan 涵道风扇
	1 fan_updown 电机
	2 fan_roll	not_using
	3 mag_in/out 舵机	0.075(stop) 0.04(push) - 0.1(pull)
	4 mag_near/far not_using
*/
uint16_t PWMPins[PWM_CHANNEL_NUM] = {\
	GPIO_Pin_6, GPIO_Pin_9, GPIO_Pin_5,\
	GPIO_Pin_10, GPIO_Pin_8};
GPIO_TypeDef *PWMPorts[PWM_CHANNEL_NUM] = {\
	GPIOG, GPIOC, GPIOG,\
	GPIOA, GPIOC};
#endif

void rcc_io_config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
}

#ifdef CAR_A_1
void pwm_config(void)
{
	float duties[PWM_CHANNEL_NUM] = {0.04, 0.075, 0.12, 0.045, 0.09};
	unsigned long freqs[PWM_CHANNEL_NUM] = {50, 50, 50, 50, 50};
	
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
#endif

#ifdef CAR_A_2
void tim14_config(void);

void pwm_config(void)
{
	float duties[PWM_CHANNEL_NUM] = {0.04, 0, 0.12, 0.075, 0.09};
	unsigned long freqs[PWM_CHANNEL_NUM] = {50, 1000, 50, 50, 50};
	
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
	
	//fan_up control gpio
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_WriteBit(GPIOG, GPIO_Pin_3, Bit_RESET);
	GPIO_WriteBit(GPIOG, GPIO_Pin_4, Bit_RESET);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	//mag_in control gpio
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	tim14_config();
}
#endif

void set_duty(uint8_t channel, float duty)
{
	PWMHighTime[channel] = PWMTotal[channel] * duty;
}

void set_freq(uint8_t channel, unsigned long freq)
{
	float duty;

	duty = (float) PWMHighTime[channel] / PWMTotal[channel];
	PWMTotal[channel] = PWM_FREQ / freq;
	PWMHighTime[channel] = PWMTotal[channel] * duty;
}

void set_duties(float duties[PWM_CHANNEL_NUM])
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
