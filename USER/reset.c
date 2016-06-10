#include "automove.h"
#include "clock.h"
#include "fan.h"
#include "movement.h"
#include "push_rod.h"
#include "switch.h"
#include "whiteline.h"

void reset1(void)//river
{
	gps_x = 0.2f;
	gps_y = 6.7f;
	gps_rad = 0;
	push_rod_c(PUSH_ROD_PUSH, 0);
	start_fan();
	delay_ms(2000);
	stop_fan();
	automove_config();
	wl_run();
	printf("wlend %d\r\n",stop_flag);
}

void reset2(void)//N2
{
	gps_x = 0;
	gps_y = 6.7f;
	gps_rad = 0;
	wl_run();
}

void reset3(void)//pole
{
	gps_x = 0.2f;
	gps_y = 6.7f;
	gps_rad = 0;
	switch_nvic_enable(SWITCH_ENTER);
}
