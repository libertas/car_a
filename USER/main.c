#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "usart.h"
#include "hpwm.h"

int main(void)
{
	systick_config();
	usart1_config(115200);
	
	hpwm_config(500,168);		//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.

	TIM_SetCompare1(TIM9,200);//修改比较值，修改占空比
	TIM_SetCompare2(TIM9,100);
	TIM_SetCompare3(TIM9,150);
	TIM_SetCompare4(TIM9,250);
	
	while(1)
	{
		//uprintf(USART1,"duty = %d\r\n",);
	}

	return 0;
}

