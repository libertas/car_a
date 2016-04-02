#include "stm32f4xx.h"
#include "link_list.h"
#include "global.h"
//控制机器人的代码中，所需要调的参数都集中放在这个结构体(param_struct结构体)中
//以后需要添加参数，首先就要在这里(param_struct结构体)添加，然后在parameter.c文件里面的param_update()函数里面添加对应的宏PARAM_UPDATE()
//每当链表里面的节点数据被改变的时候，可以执行函数param_update_all()来更新patam_struct结构体中所有成员的数据
//或者一次改变的参数不多，则可以单独调用宏PARAM_UPDATE()来更新成员的数据



typedef struct{
    float servo_p;
    float servo_i;
    float servo_d;
    float threshold;
    float servo_centroid;
    float servo_p_gain;
    float servo_d_gain;
    float servo_p_base;
    float servo_d_base;
	float return_right;
	float return_left;
	float fturn_right;
	float fturn_left;
	
	/*下面是大车的参数*/
	float acar_a;  //大车加速度档位
	float acar_v;   //大车速度档位
	float acar_wa;   //大车旋转加速度档位
	float acar_w;		//大车旋转速度档位
	float acar_xita1;  //三轮大车的轮子夹角
	float acar_xita2;
	float acar_xita3;
	float acar_l1;     //三轮大车轮子到中点的距离
	float acar_l2;
	float acar_l3;
	float gyro_x_adj;
}param_struct;

extern param_struct *g_control_param;   //这个结构体变量可被外部调用 
extern link_list g_param_list;    //参数链表

//根据链表上已有的参数数据来更新参数   USARTx 为输出错误信息的串口
#define PARAM_UPDATE(param_list,control_param_array,param_name,USARTx) \
    if(list_search(&param_list,#param_name) != NULL){    \
        int i;                  \
        for(i = 0;i < PARAM_GROUP_LENGTH;i++){     \
            control_param_array[i].param_name = list_search(&param_list,#param_name)->data->param_value[i];  	\
            }      \
    }else{   \
        uprintf(USARTx,"param_name:%s not found\n",#param_name);              \
    }                                                     \
					
					
void param_update_all();
int param_save_to_flash();
int	param_ld_from_flash();
int param_list_reset();
int param_switch(param_struct **control_param,int group_num);  //切换参数组
int param_group_now();  //获得参数组数
