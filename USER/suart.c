#include <stdbool.h>

#include "stm32f4xx_gpio.h"

#include "suart.h"

#define RXB(i) GPIO_ReadInputDataBit(su_rx_ports[i], su_rx_pins[i])

typedef unsigned char byte;
typedef unsigned int word;

uint16_t su_tx_pins[SU_CHANNEL_NUM] = {GPIO_Pin_10};
GPIO_TypeDef *su_tx_ports[SU_CHANNEL_NUM] = {GPIOA};

uint16_t su_rx_pins[SU_CHANNEL_NUM] = {GPIO_Pin_10};
GPIO_TypeDef *su_rx_ports[SU_CHANNEL_NUM] = {GPIOA};

byte TBUF[SU_CHANNEL_NUM], RBUF[SU_CHANNEL_NUM];
byte TDAT[SU_CHANNEL_NUM], RDAT[SU_CHANNEL_NUM];
byte TCNT[SU_CHANNEL_NUM], RCNT[SU_CHANNEL_NUM];
byte TBIT[SU_CHANNEL_NUM], RBIT[SU_CHANNEL_NUM];
bool TING[SU_CHANNEL_NUM], RING[SU_CHANNEL_NUM];
bool TEND[SU_CHANNEL_NUM], REND[SU_CHANNEL_NUM];

void send_uart(uint8_t channel, char data)
{
	if(TEND[channel])
	{
		TEND[channel] = 0;
		TBUF[channel] = data;
		TING[channel] = 1;
	}
}

void suputchar(uint8_t channel, char c)
{
	while(!TEND[channel]);
	send_uart(channel, c);
}

char receive_uart(uint8_t channel)
{
	char tmp = RBUF[channel];
	REND[channel] = 0;
	return tmp;
}

char sugetchar(uint8_t channel)
{
	while(!REND[channel]);
	return receive_uart(channel);
}

void uart_check(void)
{
	for(uint8_t i = 0; i < SU_CHANNEL_NUM; i++) {
		if(RING[i])
		{
			if(--RCNT[i] == 0)
			{
				RCNT[i] = 3;
				if(--RBIT[i] == 0)
				{
					RBUF[i] = RDAT[i];
					RING[i] = 0;
					REND[i] = 1;
				}
				else
				{
					RDAT[i] >>= 1;
					if(RXB(i))
						RDAT[i] |= 0x80;
				}
			}
		}
		else if(!RXB(i))
		{
			RING[i] = 1;
			RCNT[i] = 4;
			RBIT[i] = 9;
		}

		if(--TCNT[i] == 0)
		{
			TCNT[i] = 3;
			if(TING[i])
			{
				if(TBIT[i] == 0)
				{
					GPIO_WriteBit(su_tx_ports[i], su_tx_pins[i], Bit_RESET);
					TDAT[i] = TBUF[i];
					TBIT[i] = 9;
				}
				else
				{
					if(--TBIT[i] == 0)
					{
						GPIO_WriteBit(su_tx_ports[i], su_tx_pins[i], Bit_SET);
						TING[i] = 0;
						TEND[i] = 1;
					}
					else
					{
						if(TDAT[i] & 0x01)
							GPIO_WriteBit(su_tx_ports[i], su_tx_pins[i], Bit_SET);
						else
							GPIO_WriteBit(su_tx_ports[i], su_tx_pins[i], Bit_RESET);
						TDAT[i] >>= 1;
					}
				}
			}
		}
	}
}
