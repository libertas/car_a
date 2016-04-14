#include <stdint.h>

#define BRAKE_CHANNEL_NUM 2

void brake_config(void);
void brake(uint8_t channel);
void brake_release(uint8_t channel);
void emergency_stop(uint8_t channel);
