#include <stdint.h>

#include "utils.h"

#define CMD_BUF_LEN 512

extern char_queue *cmd_queue;

void interpreter_config(void);
int run_cmd(void);
