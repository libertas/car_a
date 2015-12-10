#include "misc.h"

#include "clock.h"

void systick_config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick_Config(SystemCoreClock / 1000000);
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void delay_us(unsigned long us)
{
	unsigned long old_time = SysTick->VAL;
	unsigned long now_time;
	while(us) {
		now_time = SysTick->VAL;
		us -= old_time - now_time;
		old_time = now_time;
	}
}
