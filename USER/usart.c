#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "usart.h"

/*
	The "int baud" means baud rate of the communication
	
	PA9 = TX
	PA10 = RX
	USART mode: 8-bit data, 1 stop bit, no parity and no hardware flow control
*/
void usart1_config(uint32_t baud)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
	
  USART_Cmd(USART1, ENABLE);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

#define CMD_BUFFER_LEN 100

void uprintf(USART_TypeDef* USARTx, char *fmt, ...)
{

	char buffer[CMD_BUFFER_LEN + 1];
	u8 i = 0;
	
	va_list arg_ptr;
	
	va_start(arg_ptr, fmt);  
	
	vsnprintf(buffer, CMD_BUFFER_LEN + 1, fmt, arg_ptr);
	
	USART_ClearFlag(USARTx, USART_FLAG_TXE);
	
	while ((i < CMD_BUFFER_LEN) && buffer[i])
	{
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	  USART_SendData(USARTx, (u8)buffer[i++]); 
	}
	
	va_end(arg_ptr);
}
#if 0
/*
WARNING: NOT USABLE NOW!
*/
int fputc(int c, FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART1, (uint8_t) c);
	
	return c;
}

int fgetc(FILE *fp)
{
	int ch = 0;
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	ch = (int)USART1->DR & 0xFF;

	return ch;
}
#endif


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif
