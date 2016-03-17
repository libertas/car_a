#include <stdio.h>

#include "stm32f4xx.h"

#include "auto_control.h"
#include "brake.h"
#include "can.h"
#include "clock.h"
#include "encoder.h"
#include "fan.h"
#include "interpreter.h"
#include "magnet.h"
#include "movement.h"
#include "pwm.h"
#include "switch.h"
#include "usart.h"
#include "us100.h"
#include "utils.h"
#include "push_rod.h"
#include "spi.h"
#include "suart.h"
#include "watchdog.h"

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
	watchdog_config();

	uint8_t i = 0,t = 0;
	uint8_t cnt = 0;
	//uint8_t can_send_failed;
	//uint8_t can_received;
	can1_config(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_LoopBack);//CAN初始化环回模式,波特率500Kbps    

	printf("\n\nEntering main loop\n\n");

	while(1)
	{
		if(10 == t) {
			for(i = 0; i < 8; i++)
			{
				can_buffer[i] = 0x07;//填充发送缓冲区
 			}
			can_send_failed = can1_send(can_buffer, 8);//发送8个字节
			if(can_send_failed) uprintf(USART1, "failed\r\n");
			else uprintf(USART1, "ok\r\n");
		}	 

		if(can1_receive(can_buffer))//接收到有数据
		{
			uprintf(USART1,"received\r\n");
		}
		t++;
		delay_ms(10);
		if(t==20)
		{
			t = 0;
			cnt++;
		}
	} 

	return 0;
}
