#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "fan.h"
#include "interpreter.h"
#include "movement.h"


char cmd_buf[CMD_BUF_LEN] = {0};
char_queue cmd_queue;

/*
format:
	(4-bit) [data-length] + (4-bit) [command] + (n-byte) [data] + (byte) [checksum]
	NOTE: sometimes data bytes can be used as command bits, too.
	NOTE: command 0x00 is not usable.
command list:
	move_y(int16_t arg_spd)
		(byte) 0x20 (int16) [arg_spd]

	move_x(int16_t arg_spd)
		(byte) 0x21 (int16) [arg_spd]

	move_y(float y)
		(byte) 0x40 (float) [y]

	move_x(float x)
		(byte) 0x41 (float) [x]

	move_xy_c(int8_t spd_x, int8_t spd_y)
		(byte) 0x22 (byte) [spd_x] (byte) [spd_y]

	rotate_c(int8_t r_spd)
		(byte) 0x10 (byte) r_spd

	stop()
		(byte) 0x05

	stop_all()
		(byte) 0x01

	move_up()
		(byte) 0x02

	stop_move_up()
		(byte) 0x03
	
	fan_up()
		(byte) 0x04
		
	fan_roll(float rad)
		(byte) 0x42	(float) [rad]
	
	fan_kowtow(float rad)
		(byte) 0x43 (float) [rad]
*/
int run_cmd(void)
{
	char cmd;
	out_char_queue(&cmd_queue, &cmd);
	
	uint8_t i;
	
	uint8_t buf = 0, buf1 = 0;
	uint16_t dbuf = 0;
	uint32_t qbuf = 0;
	
	float x, y, rad;
	
	
	switch(cmd) {
		default:

			#ifdef DEBUG
			printf("\nUnknown command:%x\n", cmd);
			#endif

			break;
		
		case 0x20:

			#ifdef DEBUG
			printf("\ncmd\t0x20\n");
			#endif

			out_char_queue(&cmd_queue, (char*) &buf);
			dbuf = buf;
			dbuf = dbuf << 8;
			out_char_queue(&cmd_queue, (char*) &buf);
			dbuf |= buf;

			move_y_c(dbuf);
			break;
		
		case 0x21:

			#ifdef DEBUG
			printf("\ncmd\t0x21\n");
			#endif

			out_char_queue(&cmd_queue, (char*) &buf);
			dbuf = buf;
			dbuf = dbuf << 8;
			out_char_queue(&cmd_queue, (char*) &buf);
			dbuf |= buf;

			move_x_c(dbuf);
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

		case 0x22:
			
			#ifdef DEBUG
			printf("\ncmd\t0x22\n");
			#endif

			out_char_queue(&cmd_queue, (char*) &buf);
			out_char_queue(&cmd_queue, (char*) &buf1);

			move_xy_c(buf, buf1);
			break;
		
		case 0x10:
			
			#ifdef DEBUG
			printf("\ncmd\t0x10\n");
			#endif

			out_char_queue(&cmd_queue, (char*) &buf);

			rotate_c(buf);

			break;
		
		case 0x05:
			
			#ifdef DEBUG
			printf("\ncmd\t0x00\n");
			#endif
		
			stop();
			break;
		case 0x01:
			
			#ifdef DEBUG
			printf("\ncmd\t0x01\n");
			#endif
		
			stop_all();
			break;
		case 0x02:
			
			#ifdef DEBUG
			printf("\ncmd\t0x02\n");
			#endif
		
			move_up();
			break;
		case 0x03:
			
			#ifdef DEBUG
			printf("\ncmd\t0x03\n");
			#endif
		
			stop_move_up();
			break;
		
		case 0x04:
			
			#ifdef DEBUG
			printf("\ncmd\t0x04\n");
			#endif
		
			fan_up();
			break;
		
		case 0x42:
			
			#ifdef DEBUG
			printf("\ncmd\t0x42\n");
			#endif
		
			for(i = 0; i < 4; i++) {
				out_char_queue(&cmd_queue, (char*) &buf);
				qbuf |= buf << i * 8;
			}
			memcpy(&rad, &qbuf, 4);
			printf("fan_foll(%f)\n", rad);
			fan_roll(rad);
		
			break;
		
		case 0x43:
			
			#ifdef DEBUG
			printf("\ncmd\t0x43\n");
			#endif
		
			for(i = 0; i < 4; i++) {
				out_char_queue(&cmd_queue, (char*) &buf);
				qbuf |= buf << i * 8;
			}
			memcpy(&rad, &qbuf, 4);
			//fan_kowtow(rad);
		
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
		printf("\ndata-len:%x\ncmd:0x%x\n", data_len, cmd);
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
				
				#ifdef DEBUG
				printf("\n0\n");
				#endif
		
				return 0;
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
