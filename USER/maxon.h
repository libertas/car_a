/*
ʹ��˵����
1������maxon.h
2����maxon_config���޸�maxon�����ã�Ŀǰֻ�����˼��ٶȺͼ��ٶ��Լ�����ģʽ
3��maxon_up��maxon_down��ֱ�Ϊ��ת�ͷ�ת
4��maxon_stopĿǰ�Ƿ��ٶ�Ϊ0�ķ�ʽֹͣ������disable
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
