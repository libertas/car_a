

#define MAX_MOTOR_MOV_V 4000
#define MAX_MOTOR_R_V 3500
#define MAX_MOTOR_A 200
#define MAX_V 100
#define MAX_A_CNT 200
#define MOTOR_USART USART1
#define CODER_X_TIM TIM4
#define CODER_Y_TIM	TIM5
#define CODERX_TIM_PERIOD 2000.0f
#define CODERY_TIM_PERIOD 2000.0f
#define K_CODER_X 1
#define K_CODER_Y 1
#define ACAR_RUN_ENABLE 1
#define ACAR_RUN_DISABLE 0
#define ACAR_ROTATE_P 2000
#define ACAR_ROTATE_D 1.5
#define ACAR_MOV_P 2000 
#define ACAR_MOV_D 1.5

#define PI 3.1415926535898




typedef struct{
    u8  v_lv;   //平移速度档位
    u8  a_lv;   //平移加速度档位
    u8  rv_lv;   //旋转速度档位
    u8  ra_lv;	//旋转加速度档位
    u8  run_en;   //运动使能
    
    int motor_r_v;   //当前旋转速度(量值是相对于电机来说的)
    int motor_mov_v;   //平移速度(相对于电机来说的)
    int motor1_v;  //电机速度(注意：是各个电机的速度，不是合速度)
    int motor2_v;
    int motor3_v;
    
    float purpose_mov_v;  //预期速度
    float purpose_r_v;  //预期旋转速度
    float purpose_glb_x;   //预期位置
    float purpose_glb_y;   //预期位置
    float glb_x;   //
    float glb_y;
    float distance;
    float purpose_alfa;
    float curt_alfa;   //大车相对于初始状态的旋转角度
    float curt_r_v;   //大车旋转速度
    float curt_mov_v_x;  //大车平移分速度
    float curt_mov_v_y;
    float curt_mov_v;   //当前大车的移动速度
    float mov_dir;   //大车移动的方向
    int	rotate_dir;    //-1 是逆时针  1 是顺时针  0是不转
    /******以下是大车底盘的参数(三轮)*********************/
    float xita1;   //轮轴之间的夹角
    float xita2;
    float xita3;
    float l1;      //轮子到中点的距离
    float l2;
    float l3;
    float coder_l_x;   //码盘轮子到码盘坐标系原点的距离
    float coder_l_y;
    float k_coder_x;  //码盘轮子的系数(该系数为轮子的周长),测出码盘转得的圈数，就可以知道其走的路程
    float k_coder_y;
    float c2c_l;    //车子坐标系原点到码盘坐标系原点的距离
    float c2c_xita;  //码盘的坐标系相对于大车坐标系的旋转角度
}acar_describe;


extern acar_describe g_acar_d;


acar_describe* acar_get_dec();
void acar_init();
void acar_set_a_lv(u8 a_lv);   //设置加速度档位
void acar_set_v_lv(u8 v_lv);    //设置速度档位
void acar_set_mlv(u8 a_lv,u8 v_lv);  //设置移动等级(其实就是把同设置加速度和速度)
void acar_mov(float mov_dir);    //开始移动
void acar_set_ra_lv(u8 ra_lv);   //设置旋转加速度
void acar_set_rv_lv(u8 rv_lv);   //设置旋转速度
void acar_set_rlv(u8 ra_lv,u8 rv_lv);  //同时设置加速度和旋转加速度
void acar_rotate(float rotate_angle);   //开始旋转-1 为逆时针 1为顺时针
void acar_update_alfa(float alfa);
void acar_cmd(u8 cmd);   //使能大车的运动
//acar_while()函数是不断地被执行，每次执行都扫描一遍 acar_describe 结构体，而做相应的动作
//所以，要想控制大车，改变结构体的参数即可
//该函数的参数是距离上次调用该函数的时间间隔
void acar_while(float time);   


