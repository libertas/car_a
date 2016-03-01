#include <stdint.h>

#define SU_CHANNEL_NUM 1

void suart_config(void);
char sugetchar(uint8_t channel);
void suputchar(uint8_t channel, char c);
void suart_check(void);
