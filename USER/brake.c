#include "stm32f4xx_gpio.h"

#include "brake.h"

uint16_t BrakePins[BRAKE_CHANNEL_NUM] = {GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12};

void brake_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void brake(uint16_t channel)
{
	GPIO_WriteBit(GPIOG, channel, Bit_SET);
}
