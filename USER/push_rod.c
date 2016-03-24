#include <stdint.h>

#include "stm32f4xx_tim.h"
#include "clock.h"
#include "push_rod.h"
/*if push and pull error, exchange this array*/
uint16_t PUSHPins[2 * PUSH_ROD_CHANNEL_NUM] = {\
	GPIO_Pin_14, GPIO_Pin_15,\
	GPIO_Pin_11, GPIO_Pin_13\
	};
GPIO_TypeDef *PUSHPorts[2 * PUSH_ROD_CHANNEL_NUM] = {\
	GPIOG, GPIOG,\
	GPIOG, GPIOG\
	};


void push_rod_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	for(uint8_t i = 0; i < 2* PUSH_ROD_CHANNEL_NUM; i++) {
		GPIO_InitStructure.GPIO_Pin = PUSHPins[i];
	}
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	for(uint8_t i = 0; i < 2* PUSH_ROD_CHANNEL_NUM; i++) {
		GPIO_Init(PUSHPorts[i], &GPIO_InitStructure);
	}


	for(uint8_t i = 0; i < 2* PUSH_ROD_CHANNEL_NUM; i++) {
		GPIO_WriteBit(PUSHPorts[i], PUSHPins[i], Bit_RESET);
	}
}

void push_rod_c(uint8_t dir, uint8_t num)
{
	push_rod_config();
	switch(dir){
		case 0:{
			GPIO_WriteBit(PUSHPorts[2 * num], PUSHPins[2 * num], Bit_SET);
			GPIO_WriteBit(PUSHPorts[2 * num], PUSHPins[2 * num + 1], Bit_RESET);
			break;
		}
		case 1:{
			GPIO_WriteBit(PUSHPorts[2 * num], PUSHPins[2 * num], Bit_RESET);
			GPIO_WriteBit(PUSHPorts[2 * num], PUSHPins[2 * num + 1], Bit_SET);
			break;
		}
		case 0xff:{
			GPIO_WriteBit(PUSHPorts[2 * num], PUSHPins[2 * num], Bit_RESET);
			GPIO_WriteBit(PUSHPorts[2 * num], PUSHPins[2 * num + 1], Bit_RESET);
			break;
		}
		default:break;
	}
}
void push_rod(uint8_t dir, uint8_t num)
{
	switch(dir){
		case 0:{
			push_rod_c(PUSH_ROD_PULL, num);
			break;
		}
		case 1:{
			push_rod_c(PUSH_ROD_PUSH, num);
			break;
		}
		case 0xff:{
			push_rod_c(PUSH_ROD_STOP, num);
			
			break;
		}
		default:break;
	}
	delay_ms(PUSH_ROD_TIME);
	push_rod_c(PUSH_ROD_STOP, num);
}
