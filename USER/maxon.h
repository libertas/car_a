/*
使用说明：
1、调用maxon.h
2、在maxon_config里修改maxon的配置；目前只配置了加速度和减速度以及控制模式
3、maxon_up和maxon_down里分别为正转和反转
4、maxon_stop目前是发速度为0的方式停止，不是disable
*/

void maxon_config(void);
void maxon_up(int maxon_v);
void maxon_down(int maxon_v);
void maxon_stop(void);
