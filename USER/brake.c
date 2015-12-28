#include "stm32f4xx_gpio.h"

#include "brake.h"

uint16_t brake_pins[BRAKE_CHANNEL_NUM] = {GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12};

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
	
	for(uint8_t i = 0; i < BRAKE_CHANNEL_NUM; i++) {
		GPIO_WriteBit(GPIOG, brake_pins[i], Bit_RESET);
	}
}

void brake(uint8_t channel)
{
	GPIO_WriteBit(GPIOG, brake_pins[channel], Bit_SET);
}

void brake_release(uint8_t channel)
{
	GPIO_WriteBit(GPIOG, brake_pins[channel], Bit_RESET);
}
