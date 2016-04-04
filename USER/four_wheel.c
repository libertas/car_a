#include <math.h>
#include <stdbool.h>

#include "clock.h"
#include "database.h"
#include "debug.h"
#include "encoder.h"
#include "movement.h"
#include "mti.h"
#include "utils.h"

#ifdef USE_FOUR_WHEEL

int16_t arg_speeds[4] = {0};

void f_stop(void)
{
	for(uint8_t i = 0; i < 4; i++){
		arg_speeds[i] = 0;
	}

	uprintf(USART1,\
		"\r0V%d\r1V%d\r2V%d\r5V%d\r",\
		arg_speeds[0],\
		arg_speeds[1],\
		arg_speeds[2],\
		arg_speeds[3]\
		);
}

void f_rotate(float rad)
{
	float old_rad = get_mti_value();
	if(rad > 0) {
		f_rotate_c(100);
		while(get_mti_value() < rad + old_rad);
	} else if (rad < 0) {
		f_rotate_c(-100);
		while(get_mti_value() > rad + old_rad);
	}
	stop();
}

void f_rotate_c(int8_t spd)
{
	int16_t arg_spd = (float) spd / 128 * DEFAULT_ARG_SPEED;
	arg_speeds[0] = VECT_W0 * arg_spd;
	arg_speeds[1] = VECT_W1 * arg_spd;
	arg_speeds[2] = -VECT_W2 * arg_spd;
	arg_speeds[3] = -VECT_W3 * arg_spd;
	
	#ifdef DEBUG
	printf("arg_speeds:\n");
	printf("\t0:%d\n", arg_speeds[0]);
	printf("\t1:%d\n", arg_speeds[1]);
	printf("\t2:%d\n", arg_speeds[2]);
	printf("\t3:%d\n", arg_speeds[3]);
	#endif

	uprintf(USART1,\
		"\r0V%d\r1V%d\r2V%d\r5V%d\r",\
		arg_speeds[0],\
		arg_speeds[1],\
		arg_speeds[2],\
		arg_speeds[3]\
		);
}

void f_move_arc(float y, float rad)
{
	float old_rad = get_mti_value();
	float r = y / sinf(fabsf(rad));
	float r1, r2, rt;
	float coe_y = CAR_X_LENGTH / (sqrtf( powf( CAR_X_LENGTH, 2) + powf( CAR_Y_LENGTH, 2)));
	float div_rad;

	int16_t arg_y = DEFAULT_ARG_SPEED;
	
	if(true == db_read("div_rad", (uint8_t*)&div_rad)) {
		#ifdef DEBUG
		printf("div_rad:%x\n", *(uint32_t*)&div_rad);
		printf("div_rad*10:%d\n", (uint32_t)(div_rad * 10));
	} else {
		printf("Please set div_rad\n");
		return;
		#endif
	}

	if(rad > 0) {
		r1 = (r - (float)CAR_X_LENGTH / 2000) / r;
		r2 = (r + (float)CAR_X_LENGTH / 2000) / r;
		r2 *= div_rad;
	} else if(rad < 0) {
		r1 = (r + (float)CAR_X_LENGTH / 2000) / fabs(r);
		r2 = (r - (float)CAR_X_LENGTH / 2000) / fabs(r);
		r1 *= div_rad;
	} else {
		r1 = 1;
		r2 = 1;
	}

	rt = r1 + r2;
	r1 = r1 / rt;
	r2 = r2 / rt;
	
	arg_speeds[0] = VECT_W0 * (coe_y * arg_y) * r2;
	arg_speeds[1] = VECT_W1 * (coe_y * arg_y) * r2;
	arg_speeds[2] = VECT_W2 * (coe_y * arg_y) * r1;
	arg_speeds[3] = VECT_W3 * (coe_y * arg_y) * r1;

	#ifdef DEBUG
	printf("arg_speeds:\n");
	printf("\t0:%d\n", arg_speeds[0]);
	printf("\t1:%d\n", arg_speeds[1]);
	printf("\t2:%d\n", arg_speeds[2]);
	printf("\t3:%d\n", arg_speeds[3]);
	#endif

	uprintf(USART1,\
		"\r0V%d\r1V%d\r2V%d\r5V%d\r",\
		arg_speeds[0],\
		arg_speeds[1],\
		arg_speeds[2],\
		arg_speeds[3]\
		);
	
	if(rad > 0) {
		while(get_mti_value() < rad + old_rad);
	} else if(rad < 0) {
		while(get_mti_value() > rad + old_rad);
	}
	stop();
}

void f_move_xy(float x, float y)
{
	float dest_x = get_pos_x() + x, dest_y = get_pos_y() + y;
	float t = x + y;
	f_move_xy_c(100 * x / t, 100 * y / t);

	printf("x:%f\ty:%f\n", get_pos_x(), get_pos_y());
	while(get_pos_x() - dest_x > ZERO || get_pos_y() - dest_y > ZERO) {
		#ifdef DEBUG
		printf("x:%f\ty:%f\n", get_pos_x(), get_pos_y());
		#endif
	}
	
	stop();
}

void f_move_xy_c(int8_t spd_x, int8_t spd_y)
{
	int16_t arg_x, arg_y;

	float coe_x = CAR_Y_LENGTH / (sqrtf( powf( CAR_X_LENGTH, 2) + powf( CAR_Y_LENGTH, 2)));
	float coe_y = CAR_X_LENGTH / (sqrtf( powf( CAR_X_LENGTH, 2) + powf( CAR_Y_LENGTH, 2)));


	arg_x = (int16_t) (((float) spd_x) / 128 * DEFAULT_ARG_SPEED);
	arg_y = (int16_t) (((float) spd_y) / 128 * DEFAULT_ARG_SPEED);

	arg_speeds[0] = VECT_W0 * (coe_x * arg_x + coe_y * arg_y);
	arg_speeds[1] = VECT_W1 * (-coe_x * arg_x + coe_y * arg_y);
	arg_speeds[2] = VECT_W2 * (coe_x * arg_x + coe_y * arg_y);
	arg_speeds[3] = VECT_W3 * (-coe_x * arg_x + coe_y * arg_y);

	#ifdef DEBUG
	printf("arg_speeds:\n");
	printf("\t0:%d\n", arg_speeds[0]);
	printf("\t1:%d\n", arg_speeds[1]);
	printf("\t2:%d\n", arg_speeds[2]);
	printf("\t3:%d\n", arg_speeds[3]);
	#endif

	uprintf(USART1,\
		"\r0V%d\r1V%d\r2V%d\r5V%d\r",\
		arg_speeds[0],\
		arg_speeds[1],\
		arg_speeds[2],\
		arg_speeds[3]\
		);
}

#endif
