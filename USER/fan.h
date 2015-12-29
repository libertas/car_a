#include <stdint.h>

#define FAN_CHANNEL 5

void start_fan(void);
void stop_fan(void);

void fan_up(void);
void fan_down(void);
void stop_fan_up_down(void);

void fan_kowtow(float rad);
void kowtow_check(void);

void fan_roll(float rad);
