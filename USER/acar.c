#include "acar.h"
#include "parameter.h"
#include "math.h"


acar_describe g_acar_d = {0};


acar_describe* acar_get_dec(){
    return &g_acar_d;
}


void acar_while(float d_t){
    static float glb_acar_orig_x = 0;   //未校正的大车横坐标
    static float glb_acar_orig_y = 0;   //未校正的大车纵坐标
    static float pre_alfa,pre_d_alfa;
    static float pre_distance;
    float coder_revise_x,coder_revise_y;   //因为码盘的原点与大车的原点不同，大车旋转的时候，会产生误差，所以需要校正
    float glb_revise_x,glb_revise_y;
    float d_coder_revise_x,d_coder_revise_y;
    float coder_d_x,coder_d_y;   //在单位时间间隔内，码盘走的路程(码盘坐标系)
    float acar_d_x,acar_d_y;   //单位时间间隔内，大车走的路程(未作码盘偏移校正的)(其实就是从大车坐标系下看，码盘所走的路程)
    float glb_d_x,glb_d_y;   //单位时间间隔内，从全局坐标系下看，大车走的路程
    float d_alfa,dd_alfa,d_distance;
    float mov_vector_x,mov_vector_y;   //移动矢量
    float glb_mov_dir;
    int motor_mov_v_x,motor_mov_v_y;   //相对电机来说，大车x方向的速度和y方向的速度
    
    //下面的代码是计算大车的速度和位置状态，更新的状态有:
    //mov_v_x    以大车的坐标系来观察，大车在x轴上的平移速度
    //mov_v_y    以大车的坐标系来观察，大车在y轴上的平移速度
    //mov_v      以大车的坐标系来观察，大车的平移合速度
    //current_rotare_v   大车的旋转速度
    //glb_x      以全局坐标系来观察，大车的横坐标
    //glb_y     以全局坐标系来观察，大车的纵坐标
    coder_d_x = ((CODER_X_TIM->CNT - 4000.0f)/CODERX_TIM_PERIOD) * K_CODER_X;  //位置差
    coder_d_y = ((CODER_Y_TIM->CNT - 4000.0f)/CODERY_TIM_PERIOD) * K_CODER_Y;
    CODER_X_TIM->CNT = 4000;
    CODER_Y_TIM->CNT = 4000;
    acar_d_x = coder_d_x*cos(g_acar_d.c2c_xita) - coder_d_y*sin(g_acar_d.c2c_xita);
    acar_d_y = coder_d_x*sin(g_acar_d.c2c_xita) + coder_d_y*cos(g_acar_d.c2c_xita);
    glb_d_x = acar_d_x*cos(g_acar_d.curt_alfa) - acar_d_y*sin(g_acar_d.curt_alfa);  //得出全局的路程差
    glb_d_y = acar_d_x*sin(g_acar_d.curt_alfa) + acar_d_y*cos(g_acar_d.curt_alfa);
    
    glb_acar_orig_x += glb_d_x;  //更新大车原坐标
    glb_acar_orig_y += glb_d_y;
    
    coder_revise_x = -g_acar_d.c2c_l*(1 - cos(g_acar_d.curt_alfa)) + g_acar_d.coder_l_y*g_acar_d.curt_alfa;  //算出旋转校正值
    coder_revise_y = g_acar_d.c2c_l*sin(g_acar_d.curt_alfa) + g_acar_d.coder_l_x*g_acar_d.curt_alfa;
    glb_revise_x = coder_revise_x*cos(g_acar_d.curt_alfa) - coder_revise_y*sin(g_acar_d.curt_alfa);   //算出全局校正坐标值
    glb_revise_y = coder_revise_x*sin(g_acar_d.curt_alfa) + coder_revise_y*cos(g_acar_d.curt_alfa);
    
    g_acar_d.glb_x = glb_acar_orig_x - glb_revise_x;   //原始全局坐标值，减去全局纠正坐标值，即可得到大车的真正全局坐标值
    g_acar_d.glb_y = glb_acar_orig_y - glb_revise_y;
    
    d_alfa = g_acar_d.curt_alfa - pre_alfa;  //旋转角度差分值
    g_acar_d.curt_r_v = d_alfa/d_t;    //得出旋转速度
    pre_alfa = g_acar_d.curt_alfa;
    
    d_coder_revise_x = -g_acar_d.c2c_l*(1 - cos(d_alfa)) + g_acar_d.coder_l_y*d_alfa;  //大车旋转时，码盘因旋转而多走路程的差分值
    d_coder_revise_y = g_acar_d.c2c_l*sin(d_alfa) + g_acar_d.coder_l_x*d_alfa;
    
    g_acar_d.curt_mov_v_x = (acar_d_x - d_coder_revise_x)/d_t;  //算大车的平移速度的时候，一定要减去码盘因旋转而多走的路程
    g_acar_d.curt_mov_v_y = (acar_d_y - d_coder_revise_y)/d_t;
    g_acar_d.curt_mov_v = sqrt(pow(g_acar_d.curt_mov_v_x,2) + pow(g_acar_d.curt_mov_v_y,2));   //算出合速度 
    //以上是计算大车的速度和位置的代码
    
    //算出移动向量
    mov_vector_x = g_acar_d.purpose_glb_x - g_acar_d.glb_x;
    mov_vector_y = g_acar_d.purpose_glb_y - g_acar_d.glb_y;
		//根据移动向量算出移动方向(全局的移动方向，不是从大车坐标系看的方向)
    if(mov_vector_x > 0.0001){   //算出全局的移动方向
        glb_mov_dir = atan(mov_vector_y/mov_vector_x);
    }else if(mov_vector_x < -0.0001){
        glb_mov_dir = PI + atan(mov_vector_y/mov_vector_x);
    }else{  //如果mov_vector_x == 0(其实按照程序来说，是接近于零)
        if(abs(mov_vector_y) < 0.0001){
            glb_mov_dir = 0;
        }else if(mov_vector_y > 0){
            glb_mov_dir = PI/2;   
        }else{
            glb_mov_dir = 0 - PI/2;
        }
    }
    g_acar_d.mov_dir = glb_mov_dir - g_acar_d.curt_alfa; //全局的移动方向，减去大车的旋转角度，即可得到对于大车来说的移动方向
    //到此，已经得出了大车所要移动的方向，下面根据距离通过PD算法算出移动的速度
    g_acar_d.distance = pow(pow(mov_vector_x,2) + pow(mov_vector_y,2),0.5); //移动向量的模即是所需要移动的距离
    d_alfa = g_acar_d.purpose_alfa - g_acar_d.curt_alfa;  //注意，这里所用的d_alfa变量跟上面代码所用的d_alfa变量的含义不同
    d_distance = g_acar_d.distance - pre_distance;
    dd_alfa = pre_d_alfa - d_alfa;
    pre_distance = g_acar_d.distance;
    pre_d_alfa = d_alfa;
    //以下两条语句是对大车的平移，和大车的旋转作PD算法
//    g_acar_d.motor_mov_v = ACAR_MOV_P*g_acar_d.distance + ACAR_MOV_D*d_distance;   //p算法得出平移速度的增量
//    g_acar_d.motor_r_v = (ACAR_ROTATE_P*d_alfa + ACAR_ROTATE_D*dd_alfa) * 10;   //p算法得出旋转速度的增量
		
		g_acar_d.motor_mov_v = ACAR_MOV_P*g_acar_d.distance;   //p算法得出平移速度的增量
    g_acar_d.motor_r_v = ACAR_ROTATE_P*d_alfa;   //p算法得出旋转速度的增量

    if(abs(g_acar_d.motor_mov_v) > MAX_MOTOR_MOV_V){  //限制平移速度
				if(g_acar_d.motor_mov_v > 0){
						g_acar_d.motor_mov_v = MAX_MOTOR_MOV_V;
				}else{
						g_acar_d.motor_mov_v = -MAX_MOTOR_MOV_V;
				}
    }
    if(abs(g_acar_d.motor_r_v) > MAX_MOTOR_R_V){   //限制旋转速度
				if(g_acar_d.motor_r_v > 0){
						g_acar_d.motor_r_v = MAX_MOTOR_R_V;
				}else{
						g_acar_d.motor_r_v = -MAX_MOTOR_R_V;
				}
    }

		/*****调试代码******/
	//	g_acar_d.motor_mov_v = 0;
	//	g_acar_d.motor_r_v = 0;
	//	g_acar_d.mov_dir = g_acar_d.mov_dir - PI/6;
		/*****调试代码******/
		
		
    motor_mov_v_x = g_acar_d.motor_mov_v * cos(g_acar_d.mov_dir + 5*PI/6);
    motor_mov_v_y = g_acar_d.motor_mov_v * sin(g_acar_d.mov_dir + 5*PI/6);  
    
    //算出各个电机的速度
   // g_acar_d.motor1_v = 0 - motor_mov_v_y + g_acar_d.l2*g_acar_d.motor_r_v;  
    g_acar_d.motor1_v = 0 - motor_mov_v_y  - (int)(g_acar_d.l1)*g_acar_d.motor_r_v;  
    g_acar_d.motor2_v = motor_mov_v_x*sin(g_acar_d.xita1) - motor_mov_v_y*cos(g_acar_d.xita1) - g_acar_d.l2*g_acar_d.motor_r_v;
    g_acar_d.motor3_v = 0 - motor_mov_v_x*sin(g_acar_d.xita3) - motor_mov_v_y*cos(g_acar_d.xita3) - g_acar_d.l3*g_acar_d.motor_r_v;


    if(g_acar_d.run_en == ACAR_RUN_ENABLE){   //如果大车运动被使能，则给驱动卡发指令，驱动大车
        myprintf(MOTOR_USART,"0v%d\r1v%d\r2v%d\r",g_acar_d.motor1_v,g_acar_d.motor2_v,g_acar_d.motor3_v);
    }else if(g_acar_d.run_en == ACAR_RUN_DISABLE){
        myprintf(MOTOR_USART,"v0\r");
    }
}

//更新大车旋转角度(量纲是弧度)
void acar_update_alfa(float alfa){
    g_acar_d.curt_alfa = alfa;
}

void acar_set_a_lv(u8 a_lv){
    g_acar_d.a_lv = a_lv;
}

void acar_set_v_lv(u8 v_lv){
    g_acar_d.v_lv = v_lv;
}

void acar_set_mlv(u8 a_lv,u8 v_lv){
    g_acar_d.a_lv = a_lv;
    g_acar_d.v_lv = v_lv;
}

void acar_go_xy(float go_glb_x,float go_glb_y){
    g_acar_d.purpose_glb_x = go_glb_x;
    g_acar_d.purpose_glb_y = go_glb_y;
}

void acar_mov(float mov_dir){   //mov_dir 为从大车的坐标系看，所移动的方向(角度)
    float glb_mov_dir;
    glb_mov_dir = (mov_dir/180.f)*PI - g_acar_d.curt_alfa;   //由大车坐标系的方向算得全局下的方向
    g_acar_d.purpose_glb_x = g_acar_d.glb_x + g_acar_d.v_lv*cos(glb_mov_dir); 
    g_acar_d.purpose_glb_y = g_acar_d.glb_y + g_acar_d.v_lv*sin(glb_mov_dir);
}

void acar_set_ra_lv(u8 ra_lv){
    g_acar_d.ra_lv = ra_lv;
}

void acar_set_rv_lv(u8 rv_lv){
    g_acar_d.rv_lv = rv_lv;
}

void acar_set_rlv(u8 ra_lv,u8 rv_lv){
    g_acar_d.ra_lv = ra_lv;
    g_acar_d.rv_lv = rv_lv;
}

void acar_rotate(float rotate_angle){   //入口参数为旋转的角度值，大于零是逆时针，小于零是顺时针
    g_acar_d.purpose_alfa = g_acar_d.curt_alfa + (rotate_angle/180.f)*PI;
}


void acar_cmd(u8 cmd){
    g_acar_d.run_en = cmd;
}




