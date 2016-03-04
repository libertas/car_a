#include <stdint.h>

#include "stm32f4xx_tim.h"
#include "clock.h"
#include "push_rod.h"


void push_rod_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOE, GPIO_Pin_0 | GPIO_Pin_1, Bit_RESET);
}

void push_rod_c(uint8_t dir)
{
	push_rod_config();
	switch(dir){
		case 0:{
			GPIO_WriteBit(GPIOE, GPIO_Pin_0, Bit_SET);
			GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET);
			break;
		}
		case 1:{
			GPIO_WriteBit(GPIOE, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET);
			break;
		}
		case 0xff:{
			GPIO_WriteBit(GPIOE, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET);
			break;
		}
		default:break;
	}
}
void push_rod(uint8_t dir)
{
	switch(dir){
		case 0:{
			push_rod_c(PUSH_ROD_PULL);
			break;
		}
		case 1:{
			push_rod_c(PUSH_ROD_PUSH);
			break;
		}
		case 0xff:{
			push_rod_c(PUSH_ROD_STOP);
			
			break;
		}
		default:break;
	}
	delay_ms(PUSH_ROD_TIME);
	push_rod_c(PUSH_ROD_STOP);
}
