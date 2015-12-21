#include <stdint.h>

#include "stm32f4xx_gpio.h"

#define PWM_PRESCALE (84 - 1)
#define PWM_FREQ 50000UL
#define PWM_CHANNEL_NUM 5

extern uint32_t PWMHighTime[PWM_CHANNEL_NUM];
extern uint32_t PWMTotal[PWM_CHANNEL_NUM];
extern uint16_t PWMPins[PWM_CHANNEL_NUM];
extern GPIO_TypeDef *PWMPorts[PWM_CHANNEL_NUM];

void pwm_config(void);

void set_duty(uint8_t channel, double duty);
void set_freq(uint8_t channel, unsigned long freq);

void set_duties(double duties[PWM_CHANNEL_NUM]);
void set_freqs(unsigned long freqs[PWM_CHANNEL_NUM]);
