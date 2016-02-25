#include <stdint.h>


#define VECT_EX (+1)  // encoder of x
#define VECT_EY (-1)  // encoder of y
#define VECT_EFAN (-1)  // encoder for the fan
#define VECT_FAN_UPDOWN (+1)  // encoder for the up and down movements of the fan
#define VECT_EMAG (+1)	// encoder for the up and down movements of the magnet

extern int32_t g_rotary_x;
extern int32_t g_rotary_y;
extern int32_t g_rotary_fan;
extern int32_t g_rotary_fan_updown;
extern int32_t g_rotary_mag;

void encoder_config(void);
float get_pos_fan(void);
float get_pos_x(void);
float get_pos_y(void);
float get_pos_fan_updown(void);
float get_pos_mag(void);
