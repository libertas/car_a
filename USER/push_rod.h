#include <stdint.h>

#define PUSH_ROD_PUSH 1
#define PUSH_ROD_PULL 0
#define PUSH_ROD_STOP 0xff

void push_rod_config(void);
void push_rod(uint8_t dir);
