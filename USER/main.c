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
	
	at24cxx_config();			//IIC初始化 

 	while(at24cxx_check())//检测不到24c02
	{
		uprintf(USART1,"24C02 Check Failed!");
		delay_ms(500);
		uprintf(USART1,"Please Check!      ");
		delay_ms(500);
	}
	uprintf(USART1,"24C02 Ready!");    
	while(1)
	{
		if(5 == i)//KEY1按下,写入24C02
		{
 			uprintf(USART1,"Start Write 24C02....");
			at24cxx_write(0,(u8*)TEXT_Buffer,SIZE);
			uprintf(USART1,"24C02 Write Finished!");//提示传送完成
		}
		if(10 == i)//KEY0按下,读取字符串并显示
		{
 			uprintf(USART1,"Start Read 24C02.... ");
			at24cxx_read(0,datatemp,SIZE);
			uprintf(USART1,"The Data Readed Is:  ");//提示传送完成
			uprintf(USART1,"%s\r\n",datatemp);//显示读到的字符串
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
