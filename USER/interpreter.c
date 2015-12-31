#include <stdio.h>

#include "interpreter.h"


char cmd_buf[CMD_BUF_LEN];
char_queue *cmd_queue;

/*
format:
	(4-bit) [data-length] + (4-bit) [command] + (n-byte) [data] + (byte) [checksum]
	NOTE: sometimes data bytes can be used as command bits, too
command list:
	move_y()
		(byte) 0x00
	move_x()
		(byte) 0x01
	move_y(float y)
		(byte) 0x40 (float) [y]
	move_x(float x)
		(byte) 0x41 (float) [x]
*/
int run_cmd(void)
{
	
	return 0;
}


void interpreter_config(void)
{
	init_char_queue(cmd_queue, cmd_buf, CMD_BUF_LEN);
}
