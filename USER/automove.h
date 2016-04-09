/*
Auto control will use:
	TIM10
*/

#define AUTOMOVE_DAEMON_TIME 100

void automove_config(void);
void automove_daemon(void);

float get_gps_x(void);
float get_gps_y(void);
float get_gps_rad(void);
void set_auto_dest(float x, float y, float rad);
