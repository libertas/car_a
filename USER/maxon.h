/*
使用说明：
1、调用maxon.h
2、在maxon_config里修改maxon的配置；目前只配置了加速度和减速度以及控制模式
3、maxon_up和maxon_down里分别为正转和反转
4、maxon_stop目前是发速度为0的方式停止，不是disable
*/

extern int maxon_v_l;
extern int maxon_a_l;
extern int maxon_d_l;
extern int maxon_v;
extern int maxon_c_p;
extern int maxon_c_a;
extern int maxon_c_c;
extern int maxon_b;
extern int maxon_c_rate;
extern int maxon_c_out;
extern int maxon_g;
extern int maxon_faststop;

void maxon_config(void);
void maxon_up(void);
void maxon_down(void);
void maxon_stop(void);
void maxon_current(void);
void maxon_baud(void);
void maxon_init(void);
void maxon_current_move(void);
void maxon_ask(void);
