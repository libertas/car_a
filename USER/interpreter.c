#include <stdio.h>

#include "interpreter.h"


char g_cmd_buf[CMD_BUF_LEN] = {0};
char cmd_buf[CMD_BUF_LEN] = {0};

/*
command list:
	move_y()
		(byte) 0x00 (byte) 0x0d
	move_x()
		(byte) 0x01 (byte) 0x0d
	move_y(float y)

	move_x(float x)

*/
int run_cmd(void)
{
	if(1 != sscanf(g_cmd_buf, "%s\r", cmd_buf)) {
		return -1;
	}
	
	for(uint16_t i = 0; i < CMD_BUF_LEN; i++) {
		g_cmd_buf[i] = 0;
	}
	return 0;
}
