
//#define USE_PID_INC

#ifndef USE_PID_INC
typedef struct {
    float set_value;
    float actual_value;
    float error;
    float error_old;
    float kp,ki,kd;
    float sum;
}pid_inittypedef;
#endif

#ifdef USE_PID_INC
typedef struct {
    float set_value;
    float actual_value;
    float error;
    float error_oold;
    float error_old;
    float kp,ki,kd;
    float sum;
}pid_inittypedef;
#endif

void pid_config(pid_inittypedef* pid_initstruct);
float pid_realize(pid_inittypedef* pid);

/*example**************usage like GPIO_Init*******
#include "pid.h"

int main()
{
    printf("System begin \n");
    pid_inittypedef pid_initstructure;

    pid_initstructure.set_value = 100.0;
    pid_initstructure.kp = 0.2;
    pid_initstructure.ki = 0.015;
    pid_initstructure.kd = 0.2;
    pid_config(&pid_initstructure);

    int count=0;
    while(count<1000)
    {
        float actual = pid_realize(&pid_initstructure);
        printf("%f\r\n",actual);
        count++;
    }
    return 0;
}
*/
