#define SWITCH_CHANNEL_NUM 4

/*
	0 PE8  fan_down_stop
	1 PF11 fan_up_stop
	2 PF3  light_electricity(12V)
	3 PF9  move_up_stop//light_electricity(12V)
*/

void switch_config(uint8_t i);
void switch_nvic_enable(uint8_t i);
void switch_nvic_disable(uint8_t i);
