#include <stdint.h>

#define FAN_CHANNEL 5
#define FAN_RMOVE_DIST ((float) 0.1)

void start_fan(void);
void stop_fan(void);

void fan_up(void);
void fan_down(void);
void stop_fan_up_down(void);

void fan_kowtow(float rad);
void fan_kowtow_r(int8_t dir);  // +1 or -1
void fan_kowtow_stop(void);
void kowtow_check(void);

void fan_roll(float rad);
void fan_roll_r(int8_t dir);  // +1 or -1
