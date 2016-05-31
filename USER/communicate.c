#include "communicate.h"
#include "interpreter.h"
#include "can.h"
#include "vega.h"

void comm_msg_callback(CanRxMsg *can_rx_msg);

void comm_init(void)
{
		can_init(comm_msg_callback);
}	

void comm_msg_callback(CanRxMsg *can_rx_msg)
{
		data_convert temp;
		uint8_t length;
		char data;
    if(can_rx_msg->StdId == COMM_REC_ID){   //???VEGA??
				length = can_rx_msg->DLC;
				for(int i = 0; i <length; i++){
						temp.u8_form[i] = can_rx_msg->Data[i];
						data = can_rx_msg->Data[i];
				}
        
    } 
}
