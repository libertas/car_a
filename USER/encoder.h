#include <stdint.h>

#include "car.h"


#define ENCODER_CHANNEL_NUM 5

#define VECT_EX (-1)  // encoder of x
#define VECT_EY (+1)  // encoder of y
#define VECT_EFAN (-1)  // encoder for the fan

#ifdef CAR_A_1
#define EX_X 0.21f
#define EX_Y 0.065f
#define EY_X 0.21f
#define EY_Y -0.065f
#endif

#ifdef CAR_A_2
#define EX_X 0.0435f
#define EX_Y -0.1655f
#define EY_X -0.06f
#define EY_Y -0.1675f
#endif


extern int32_t g_rotary_x;
extern int32_t g_rotary_y;
extern int32_t g_rotary_fan;


void encoder_config(void);
void encoder_test(void);
float get_pos_fan(void);
float get_pos_x(void);
float get_pos_y(void);
