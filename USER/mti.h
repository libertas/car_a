#include "dma.h"
#include "string.h"

#define ANGLE_TO_RAD (float)0.01745

extern const uint8_t mti_flag[4] ;
extern float mti_roll, mti_pitch, mti_yaw;

float mti(void);
int btol(uint8_t buffer[]);
