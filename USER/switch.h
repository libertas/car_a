#define SWITCH_CHANNEL_NUM 4

#define SWITCH_NEAR 0

/*
	0 PF9  switch_too_near(12V)
	1 PF11 fan_up_stop
	2 PF3  light_electricity(12V)
	3 PF9  move_up_stop//light_electricity(12V)
*/

void switch_config(uint8_t i);
void switch_nvic_enable(uint8_t i);
void switch_nvic_disable(uint8_t i);
