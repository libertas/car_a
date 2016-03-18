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

	iic_config();
	uint16_t t = 0;
	uint8_t datatemp[SIZE];
	
	printf("\n\nEntering main loop\n\n");
	while(AT24CXX_Check())//检测不到24c02
	{
		uprintf(USART3,"at24 Check Failed!");
		delay_ms(500);
		uprintf(USART3,"Please Check!      ");
		delay_ms(500);
	}
	uprintf(USART3,"at24 Ready!");      
	while(1)
	{
		if(10 == t)
		{
 			uprintf(USART3,"Start Write at24....");
			AT24CXX_Write(0,(uint8_t*)TEXT_Buffer,SIZE);
			uprintf(USART3,"at24 Write Finished!");
		}
		if(20 == t)
		{
 			uprintf(USART3,"Start Read at24.... ");
			AT24CXX_Read(0,datatemp,SIZE);
			uprintf(USART3,"The Data Readed Is:  ");//提示传送完成
			uprintf(USART3,"%d",datatemp);//显示读到的字符串
		}
		t++;
		delay_ms(10);
		if(30 == t)
		{
			t = 0;
		}		   
	} 	   

	return 0;
}
