#include "global.h"
#include "usart.h"

const uint8_t flag[4] = { 0xFA,0xFF,0x32,0x0E};
uint8_t buffer[BUF_SIZE] = {'0'};
uint8_t sendbuffer[SEND_SIZE] = {0xFA,0x01,0xA4,0x02,0x00,0x04,0x55};
float roll = 0;
float pitch = 0;
float yaw = 0;
