#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "usart.h"

void tmp_config(void);

int main(void)
{
	systick_config();
	usart1_config(115200);
	tmp_config();
	
	while(1)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_12); 
		delay_ms(1);
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		delay_ms(1);
	}

	return 0;
}

void tmp_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
