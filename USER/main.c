#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "usart.h"
#include "hpwm.h"

int main(void)
{
	systick_config();
	usart1_config(115200);
	
	TIM9_PWM_Init(500,84);		//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.

	TIM_SetCompare1(TIM9,200);//�޸ıȽ�ֵ���޸�ռ�ձ�
	
	while(1)
	{
		
	}

	return 0;
}

