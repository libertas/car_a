#include <stdint.h>


#define VECT_EX (-1)  // encoder of x
#define VECT_EY (-1)  // encoder of y
#define VECT_EFAN (-1)  // encoder for the fan

extern int32_t g_rotary_x;
extern int32_t g_rotary_y;
extern int32_t g_rotary_fan;

void encoder_config(void);
float get_pos_fan(void);
float get_pos_x(void);
float get_pos_y(void);
