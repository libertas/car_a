#include <stdlib.h>
#include <stdint.h>
#include "pid.h"

#ifndef USE_PID_INC
void pid_config(pid_inittypedef* pid_initstruct)
{
	pid_initstruct->actual_value = 0.0;
	pid_initstruct->error = 0.0;
	pid_initstruct->error_old = 0.0;
	pid_initstruct->sum = 0.0;
}

float pid_realize(pid_inittypedef* pid)
{
    int8_t index = 0;
    if(abs(pid->error) > (pid->set_value * 1.5F)) {
        index = 0;
    }
    else {
        index = 1;
        pid->sum += pid->error;
    }
	pid->error = pid->set_value - pid->actual_value;
	pid->actual_value = pid->kp * (pid->error)\
                      + pid->ki * (pid->sum) * (index)\
                      + pid->kd * (pid->error - pid->error_old);
	pid->error_old = pid->error;
	return pid->actual_value;
}
#endif

#ifdef USE_PID_INC
void pid_config(pid_inittypedef* pid_initstruct)
{
	pid_initstruct->actual_value = 0.0;
	pid_initstruct->error = 0.0;
	pid_initstruct->error_oold = 0.0;
	pid_initstruct->error_old = 0.0;
	pid_initstruct->sum = 0.0;
}

float pid_realize(pid_inittypedef* pid)
{
    pid->error = pid->set_value - pid->actual_value;
    float increment = pid->kp * (pid->error - pid->error_old)\
                    + pid->ki * (pid->error)\
                    + pid->kd * (pid->error - 2 * pid->error_old + pid->error_oold);
    pid->actual_value += increment;
    pid->error_oold = pid->error_old;
    pid->error_old = pid->error;
return pid->actual_value;
}
#endif
