#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "usart.h"
#include "hpwm.h"

int main(void)
{
	systick_config();
	usart1_config(115200);
	
	TIM9_PWM_Init(500,168);		//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.

	TIM_SetCompare1(TIM9,200);//修改比较值，修改占空比
	
	while(1)
	{
		
	}

	return 0;
}

