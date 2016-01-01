#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "interpreter.h"
#include "movement.h"


char cmd_buf[CMD_BUF_LEN] = {0};
char_queue cmd_queue;

/*
format:
	(4-bit) [data-length] + (4-bit) [command] + (n-byte) [data] + (byte) [checksum]
	NOTE: sometimes data bytes can be used as command bits, too
command list:
	move_y()
		(byte) 0x10 (byte) [dir]
	move_x()
		(byte) 0x11 (byte) [dir]
	move_y(float y)
		(byte) 0x40 (float) [y]
	move_x(float x)
		(byte) 0x41 (float) [x]
*/
int run_cmd(void)
{
	char cmd;
	out_char_queue(&cmd_queue, &cmd);
	
	uint8_t i;
	
	uint8_t buf = 0;
	uint32_t qbuf = 0;
	
	float x, y;
	
	
	switch(cmd) {
		default:

			#ifdef DEBUG
			printf("\nUnknown command:%x\n", cmd);
			#endif

			break;
		
		case 0x10:

			#ifdef DEBUG
			printf("\ncmd\t0x10\n");
			#endif

			out_char_queue(&cmd_queue, (char*) &buf);
			move_y_c(buf);
			break;
		
		case 0x11:

			#ifdef DEBUG
			printf("\ncmd\t0x11\n");
			#endif

			out_char_queue(&cmd_queue, (char*) &buf);
			move_x_c(buf);
			break;
		
		case 0x40:

			#ifdef DEBUG
			printf("\ncmd\t0x40\n");
			#endif

			for(i = 0; i < 4; i++) {
				out_char_queue(&cmd_queue, (char*) &buf);
				qbuf = qbuf << 8;
				qbuf |= buf;
			}
			memcpy(&y, &qbuf, 4);
			move_y(y);
			break;
		
		case 0x41:

			#ifdef DEBUG
			printf("\ncmd\t0x41\n");
			#endif

			for(i = 0; i < 4; i++) {
				out_char_queue(&cmd_queue, (char*) &buf);
				qbuf = qbuf << 8;
				qbuf |= buf;
			}
			memcpy(&x, &qbuf, 4);
			move_x(x);
			break;
	}
	
	return 0;
}

int check_cmd(void)
{
	uint16_t fr;
	uint8_t data_len, cmd;
	uint8_t check_sum = 0;
	uint16_t i;
	
	if(cmd_queue.count > 0) {
		fr = (cmd_queue.front + 1) % cmd_queue.max_size;
		cmd = cmd_queue.data[fr];
		data_len = (cmd >> 4);
		cmd &= 0x0f;
		
		#ifdef DEBUG
		printf("\nlen:%x\ncmd:0x%x\n", data_len, cmd);
		#endif
		
		if(cmd_queue.count >= data_len + 2) {
			for(i = fr; i <= (fr + data_len) % cmd_queue.max_size; i = (i + 1) % cmd_queue.max_size) {
				check_sum += cmd_queue.data[i];
			}
			if(cmd_queue.data[i] == check_sum) {
				
				#ifdef DEBUG
				printf("\nrun_cmd()\n");
				#endif
				
				run_cmd();
				out_char_queue(&cmd_queue, (char*) &check_sum);  // remove the check_sum byte
			} else {
				
				#ifdef DEBUG
				printf("\n-3\n");
				#endif
				
				out_char_queue(&cmd_queue, (char*) &check_sum);  //remove the wrong byte
				
				return -3;
			}
		} else {
			
			#ifdef DEBUG
			printf("\n-2\n");
			#endif
			
			return -2;
		}
		
		#ifdef DEBUG
		printf("\n0\n");
		#endif
		
		return 0;
	} else {
	
		#ifdef DEBUG
		// printf("\n-1\n");
		#endif
		
		return -1;
	}
}


void interpreter_config(void)
{
	init_char_queue(&cmd_queue, cmd_buf, CMD_BUF_LEN);
}
