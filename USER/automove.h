/*
Auto control will use:
	TIM10
*/

#define AUTOMOVE_DAEMON_TIME 10

void automove_config(void);
void automove_daemon(void);

float get_gps_x(void);
float get_gps_y(void);
float get_gps_rad(void);
