#include "dma.h"
#include "string.h"

extern const uint8_t mti_flag[4] ;
extern float mti_roll, mti_pitch, mti_yaw;

void mti(void);
int btol(uint8_t buffer[]);
