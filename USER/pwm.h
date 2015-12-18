#include <stdint.h>

#include "stm32f4xx_gpio.h"

#define PWM_PRESCALE (168 - 1)
#define PWM_FREQ 50000UL
#define PWM_CHANNEL_NUM 5

extern uint32_t PWMHighTime[PWM_CHANNEL_NUM];
extern uint32_t PWMTotal[PWM_CHANNEL_NUM];
extern uint16_t PWMPins[PWM_CHANNEL_NUM];
extern GPIO_TypeDef *PWMPorts[PWM_CHANNEL_NUM];

void pwm_config(void);

void set_all_duty(double d);
void set_all_freq(unsigned long f);

void set_duty(double duty[PWM_CHANNEL_NUM]);
void set_freq(unsigned long freq[PWM_CHANNEL_NUM]);
