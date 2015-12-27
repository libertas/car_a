#include <stdint.h>

#define CMD_BUF_LEN 512

extern char g_cmd_buf[CMD_BUF_LEN];

int run_cmd(void);
