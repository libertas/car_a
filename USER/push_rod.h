#include <stdint.h>

#define PUSH_ROD_PUSH 1
#define PUSH_ROD_PULL 0
#define PUSH_ROD_STOP 0xff
#define PUSH_ROD_TIME 100 //µ¥Î»£ºms
#define PUSH_ROD_CHANNEL_NUM 2

void push_rod_c(uint8_t dir, uint8_t num);
void push_rod(uint8_t dir, uint8_t num);
