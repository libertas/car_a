#include <stdint.h>

#define PUSH_ROD_PUSH 1
#define PUSH_ROD_PULL 0
#define PUSH_ROD_STOP 0xff
#define PUSH_ROD_TIME 1000 //µ¥Î»£ºms

void push_rod_c(uint8_t dir);
void push_rod(uint8_t dir);
