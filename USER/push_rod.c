#include <stdint.h>

#include "stm32f4xx_tim.h"
#include "car.h"
#include "clock.h"
#include "push_rod.h"

#ifdef CAR_A_1
// 0 PG14/15 靠近核心板
// 1 PG11/13 板子少的一边
uint16_t PUSHPins[2 * PUSH_ROD_CHANNEL_NUM] = {\
	GPIO_Pin_1, GPIO_Pin_0,\
	GPIO_Pin_13, GPIO_Pin_11\
	};

GPIO_TypeDef *PUSHPorts[2 * PUSH_ROD_CHANNEL_NUM] = {\
	GPIOE, GPIOE,\
	GPIOG, GPIOG\
	};
#endif


#ifdef CAR_A_2
//four gas
/*
	0	PG11	FAN_ROLL
	1	PG13	cat_ear
	2	PG14	push_rod
	3	for left\right	not_using
*/
uint16_t PUSHPins[2 * PUSH_ROD_CHANNEL_NUM] = {\
	GPIO_Pin_11, GPIO_Pin_13,\
	GPIO_Pin_14, GPIO_Pin_15\
	};

GPIO_TypeDef *PUSHPorts[2 * PUSH_ROD_CHANNEL_NUM] = {\
	GPIOG, GPIOG,\
	GPIOG, GPIOG\
	};
#endif


void push_rod_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	for(uint8_t i = 0; i < 2* PUSH_ROD_CHANNEL_NUM; i++) {
		GPIO_InitStructure.GPIO_Pin = PUSHPins[i];
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(PUSHPorts[i], &GPIO_InitStructure);
	}

	for(uint8_t i = 0; i < 2* PUSH_ROD_CHANNEL_NUM; i++) {
		GPIO_WriteBit(PUSHPorts[i], PUSHPins[i], Bit_RESET);
	}
}

#ifdef CAR_A_1
void push_rod_c(uint8_t dir, uint8_t num)
{
	switch(dir){
		case 0:{
			GPIO_WriteBit(PUSHPorts[2 * num], PUSHPins[2 * num], Bit_SET);
			GPIO_WriteBit(PUSHPorts[2 * num + 1], PUSHPins[2 * num + 1], Bit_RESET);
			break;
		}
		case 1:{
			GPIO_WriteBit(PUSHPorts[2 * num], PUSHPins[2 * num], Bit_RESET);
			GPIO_WriteBit(PUSHPorts[2 * num + 1], PUSHPins[2 * num + 1], Bit_SET);
			break;
		}
		case 0xff:{
			GPIO_WriteBit(PUSHPorts[2 * num], PUSHPins[2 * num], Bit_RESET);
			GPIO_WriteBit(PUSHPorts[2 * num + 1], PUSHPins[2 * num + 1], Bit_RESET);
			break;
		}
		default:break;
	}
}

void push_rod(uint8_t dir, uint8_t num)
{
	push_rod_c(dir, num);
	if(1 == num) delay_ms(PUSH_ROD_TIME);
	else delay_ms(PUSH_ROD_TIME / 5);
	push_rod_c(PUSH_ROD_STOP, num);
}
#endif

#ifdef CAR_A_2
void push_rod_c(uint8_t dir, uint8_t num)
{
	switch(dir) {
		case 1:
			GPIO_WriteBit(PUSHPorts[num], PUSHPins[num], Bit_SET);
			break;
		case 0:
			GPIO_WriteBit(PUSHPorts[num], PUSHPins[num], Bit_RESET);
			break;
		default:
			GPIO_WriteBit(PUSHPorts[num], PUSHPins[num], Bit_RESET);
			break;
	}
}
void push_rod(uint8_t dir, uint8_t num)
{
	if(1 == dir) {
		push_rod_c(dir, num);
		delay_ms(PUSH_ROD_TIME);
	}
	push_rod_c(PUSH_ROD_STOP, num);
}
#endif
