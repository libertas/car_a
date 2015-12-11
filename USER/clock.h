#include <stdint.h>

void delay_init(uint8_t SYSCLK);
void systick_config(void);
void delay_us(uint32_t us);
