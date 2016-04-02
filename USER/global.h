#include <stdio.h>
#include "stm32f4xx.h"
#include "string.h"

#define BUF_SIZE 19
#define SEND_SIZE 7

extern const uint8_t flag[4] ;
extern uint8_t buffer[BUF_SIZE];
extern uint8_t sendbuffer[SEND_SIZE];
extern char test[10];
extern float roll, pitch, yaw;
extern uint8_t c;
