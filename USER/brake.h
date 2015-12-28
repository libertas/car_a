#include <stdint.h>

#define BRAKE_CHANNEL_NUM 3

void brake_config(void);
void brake(uint8_t channel);
void brake_release(uint8_t channel);
