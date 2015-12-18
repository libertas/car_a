#include <stdint.h>

#define PWM_PRESCALE (168 - 1)

#define PWM_FREQ 50000UL

extern uint8_t PWMState;
extern uint32_t PWMHighTime;
extern uint32_t PWMLowTime;
extern uint32_t PWMTotal;

void pwm_config(void);
void set_duty(double duty);
void set_freq(unsigned long freq);
