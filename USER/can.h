#ifndef __CAN_H
#define __CAN_H	
#include <stdint.h>
  
	
//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    
										 							 				    
uint8_t can1_config(uint8_t tsjw, uint8_t tbs2, uint8_t tbs1, uint16_t brp, uint8_t mode);
uint8_t can1_send(uint8_t* msg, uint8_t len);
uint8_t can1_receive(uint8_t *buf);
#endif

extern uint8_t can_buffer[8];
extern uint8_t can_send_failed;
