#include "maxon.h"
#include "usart.h"
#include "parameter.h"

int maxon_v_l = 0;
int maxon_a_l = 200;
int maxon_d_l = 40;
int maxon_v = 0;
int maxon_c_p = 1800;
int maxon_c_a = 1000;
int maxon_c_c = 600;
int maxon_b = 9600;
int maxon_c_rate = 4000;
int maxon_c_out = 200;
int maxon_g;
int maxon_faststop = 20;

param_struct *maxon_param;

void maxon_init(void)
{
		param_init(&maxon_param);
}

void maxon_config(void)
{
		maxon_a_l = (int)maxon_param -> maxon_a_l;
		maxon_d_l = (int)maxon_param -> maxon_d_l;
		maxon_faststop = (int)maxon_param -> maxon_faststop;
		uprintf(USART1, "s r0x24 11\r\n\n");
		uprintf(USART1, "s r0x36 %d\r\n\n", maxon_a_l);
		uprintf(USART1, "s r0x37 %d\r\n\n", maxon_d_l);		
		uprintf(USART1, "s r0x39 %d\r\n\n", maxon_faststop);
}

void maxon_up(void)
{
		maxon_v = (int)maxon_param -> maxon_v;
		uprintf(USART1, "s r0x2f %d\r\n\n", maxon_v);
}

void maxon_down(void)
{
		maxon_v = (int)maxon_param -> maxon_v;
		uprintf(USART1, "s r0x2f -%d\r\n\n", maxon_v);
}

void maxon_stop(void)
{
		uprintf(USART1, "s r0x2f 0\r\n\n");
		uprintf(USART1, "s r0x24 0\r\n\n");
}

void maxon_current(void)
{
		maxon_c_a = (int)maxon_param -> maxon_c_a;
		maxon_c_p = (int)maxon_param -> maxon_c_p;
		maxon_c_c = (int)maxon_param -> maxon_c_c;
		uprintf(USART1, "s r0x23 %d\r\n\n", maxon_c_a);
		uprintf(USART1, "s r0x21 %d\r\n\n", maxon_c_p);
		uprintf(USART1, "s r0x22 %d\r\n\n", maxon_c_c);
}

void maxon_baud(void)
{
		maxon_b = (int)maxon_param -> maxon_b;
		uprintf(USART1, "s r0x90 %d\r\n\n", maxon_b);
		uart1_config(maxon_b);
}

void maxon_current_move(void)
{
		maxon_c_rate = (int)maxon_param -> maxon_c_rate;
		maxon_c_out = (int)maxon_param -> maxon_c_out;
		uprintf(USART1, "s r0x6a %d\r\n\n", maxon_c_rate);
		uprintf(USART1, "s r0x02 %d\r\n\n", maxon_c_out);
		uprintf(USART1, "s r0x24 1\r\n\n");
}

void maxon_ask(void)
{
	
}
