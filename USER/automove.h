/*
Auto control will use:
	TIM10
*/

#include <stdbool.h>
#include <stdint.h>

#define MAX_ROTATE_SPD 1000
#define MAX_XY_SPD 2000
#define AUTO_NEAR_DIST 0.1f

extern bool automove_flag;
extern bool auto_continous_flag;
extern uint16_t ROTATE_DEFAULT_SPD;
extern uint16_t XY_DEFAULT_SPD;

void automove_config(void);
void automove_daemon(void);
void automove_disable(void);
bool near_auto_dest(void);

float get_gps_x(void);
float get_gps_y(void);
float get_gps_rad(void);
void set_auto_dest(float x, float y, float rad);

extern bool automove_flag;
