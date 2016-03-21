#include <stdio.h>

#include "stm32f4xx.h"

#include "at24.h"
#include "auto_control.h"
#include "brake.h"
#include "clock.h"
#include "encoder.h"
#include "fan.h"
#include "flash.h"
#include "interpreter.h"
#include "magnet.h"
#include "movement.h"
#include "multi_processing.h"
#include "pwm.h"
#include "switch.h"
#include "usart.h"
#include "us100.h"
#include "utils.h"
#include "push_rod.h"
#include "spi.h"
#include "suart.h"
#include "watchdog.h"


const uint8_t TEXT_Buffer[]={"sbxd STM32F4 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)
int main(void)
{
	system_clk_set();
	systick_config();

	interpreter_config();
	uart_config(115200);
	spi_config();
	suart_config();

	brake_config();
	encoder_config();
	// switch_config();
	pwm_config();
	// watchdog_config();

	u16 i=0;
	u8 datatemp[SIZE];	
	
	at24cxx_config();			//IIC��ʼ�� 

 	while(at24cxx_check())//��ⲻ��24c02
	{
		uprintf(USART1,"24C02 Check Failed!");
		delay_ms(500);
		uprintf(USART1,"Please Check!      ");
		delay_ms(500);
	}
	uprintf(USART1,"24C02 Ready!");    
	while(1)
	{
		if(5 == i)//KEY1����,д��24C02
		{
 			uprintf(USART1,"Start Write 24C02....");
			at24cxx_write(0,(u8*)TEXT_Buffer,SIZE);
			uprintf(USART1,"24C02 Write Finished!");//��ʾ�������
		}
		if(10 == i)//KEY0����,��ȡ�ַ�������ʾ
		{
 			uprintf(USART1,"Start Read 24C02.... ");
			at24cxx_read(0,datatemp,SIZE);
			uprintf(USART1,"The Data Readed Is:  ");//��ʾ�������
			uprintf(USART1,"%s\r\n",datatemp);//��ʾ�������ַ���
		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			i=0;
		}		   
	} 		   

	return 0;
}
