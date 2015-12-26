#include <stdio.h>

#include "stm32f4xx.h"

#include "clock.h"
#include "encoder.h"
#include "hpwm.h"
#include "i2c.h"
#include "movement.h"
#include "mpu6050.h"
#include "pwm.h"
#include "switch.h"
#include "usart.h"

int main(void)
{
	systick_config();

	uart_config(115200);

	encoder_config();
	i2c1_config();
	mpu6050_config();
	switch_config();

	pwm_config();


	// move_y(314);

	printf("\n\nEntering main loop\n\n");
	while(1)
	{
		printf("%d\n", mpu6050_get_exact_data(ACCEL_XOUT_H));
	}

	return 0;
}
