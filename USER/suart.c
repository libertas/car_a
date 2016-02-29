#include "suart.h"

typedef __bit bool;
typedef unsigned char byte;
typedef unsigned int word;

byte TBUF, RBUF;
byte TDAT, RDAT;
byte TCNT, RCNT;
byte TBIT, RBIT;
bool TING, RING;
bool TEND, REND;

void sendUART(unsigned char data)
{
	if(TEND)
	{
		TEND = 0;
		TBUF = data;
		TING = 1;
	}
}

void putchar(char c)
{
	while(!TEND);
	sendUART(c);
}

unsigned char receiveUART()
{
	unsigned char tmp = RBUF;
	REND = 0;
	return tmp;
}

unsigned char getChar()
{
	while(!REND);
	return receiveUART();
}

void uart() __interrupt(TF1_VECTOR)
{
	if(RING)
	{
		if(--RCNT == 0)
		{
			RCNT = 3;
			if(--RBIT == 0)
			{
				RBUF = RDAT;
				RING = 0;
				REND = 1;
			}
			else
			{
				RDAT >>= 1;
				if(RXB)
					RDAT |= 0x80;
			}
		}
	}
	else if(!RXB)
	{
		RING = 1;
		RCNT = 4;
		RBIT = 9;
	}

	if(--TCNT == 0)
	{
		TCNT = 3;
		if(TING)
		{
			if(TBIT == 0)
			{
				TXB = 0;
				TDAT = TBUF;
				TBIT = 9;
			}
			else
			{
				if(--TBIT == 0)
				{
					TXB = 1;
					TING = 0;
					TEND = 1;
				}
				else
				{
					TDAT >>= 1;
					TXB = CY;
				}
			}
		}
	}
}
