#include "parameter.h"


param_struct *g_control_param;   //存放控制参数的结构体
static param_struct control_param_array[PARAM_GROUP_LENGTH];   //定义参数组
link_list g_param_list = NULL;     //存放控制参数的链表
static int group_now;


//在自定义结构体param_struct里添加成员参数之后，需要在这个函数里添加相应的宏PARAM_UPDATE()
void param_update_all(){
//    PARAM_UPDATE(g_param_list,control_param_array,servo_p,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,servo_i,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,servo_d,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,threshold,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,servo_centroid,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,servo_p_gain,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,servo_d_gain,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,servo_p_base,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,servo_d_base,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,return_right,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,return_left,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,fturn_left,UART5);
//    PARAM_UPDATE(g_param_list,control_param_array,fturn_right,UART5);
    PARAM_UPDATE(g_param_list,control_param_array,gyro_x_adj,UART5);
}

//
int param_list_reset(){
    if(g_param_list != NULL){
        list_clear(&g_param_list);
    }else{				
        if(list_init(&g_param_list) == -1){
            return -1;
        }
    }	
    return 1;
}

int param_group_now(){
    return group_now;
}

//切换参数组
int param_switch(param_struct **control_param,int group_num){
    if(group_num > PARAM_GROUP_LENGTH - 1 || group_num < 0){
        return -1;
    }
    *control_param = &(control_param_array[group_num]);
    group_now = group_num;
    return 1;
}

//从FLASH加载控制参数到链表，并且更新参数结构体的成员
int param_ld_from_flash(){
    static char param_name[PARAM_NAME_LENGTH];  //估计栈空间被占用了。。。
    float *param_value;
    char c_read_temp;
    int i,str_index;
    u32 param_addr = PARAM_FLASH_ADDR_START;
    if(g_param_list != NULL){
        list_clear(&g_param_list);
    }else{   //判断链表有没有被初始化
        if(list_init(&g_param_list) == -1){
                return -1;
        }
    }
    
    while(1){
        //加载参数的名称
        str_index = 0;
        memset(param_name,0,sizeof(char)*PARAM_NAME_LENGTH);
        for(i = 0;i < PARAM_NAME_LENGTH-1;i++){
            c_read_temp = *((u8 *)param_addr);
            param_name[i] = c_read_temp;
            param_addr++;
        }
        param_addr++;
        if(strcmp(param_name,"end") == 0){
            break;   //end为结束标志
        }
        param_value = (float *)param_addr;  // 加载参数值
        param_addr = param_addr + 4 * PARAM_GROUP_LENGTH;
        if(param_addr > PARAM_FLASH_ADDR_END){  //如果超出参数中FLASH的存储区域，则报错
            return -2;
        }
        list_insert(&g_param_list,list_get_length(&g_param_list)+1,param_name,param_value); //把参数插入链表
    }
    param_update_all();
    return 1;
}

int param_save_to_flash(){
    char *param_name;
    char param_end[] = "end";
    u8 *c_write;
    float *param_value;
    int i,j,str_index;
    u32 param_addr = PARAM_FLASH_ADDR_START;
    list_node *p = g_param_list->link;
    
    FLASH_Unlock();  //解锁FLASH，写数据
    FLASH_DataCacheCmd(DISABLE);
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
    								FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);  //写FLASH之前，先清除标志
    if(FLASH_EraseSector(PARAM_FLASH_SETOR, VoltageRange_3) != FLASH_COMPLETE){ 
        FLASH_DataCacheCmd(ENABLE); 
        FLASH_Lock();//上锁
        return -1;   //擦写失败，，退出
    }
    while(p != NULL){
        param_name = p->data->param_name;
        param_value = p->data->param_value;
        str_index = 0;
        for(i = 0;i < PARAM_NAME_LENGTH;i++){
            FLASH_ProgramByte(param_addr,param_name[str_index++]);
            param_addr++;
        }
        for(j = 0;j < PARAM_GROUP_LENGTH;j++){
            c_write = (u8 *)(&(param_value[j]));   //参数值是实数型的，一个参数值占用4个字节，然而不能直接烧到FLASH里，只能按字节烧进去
            for(i = 0;i < 4;i++){
                FLASH_ProgramByte(param_addr++,*(c_write++));
            }
        }
        if(param_addr > PARAM_FLASH_ADDR_END){   //如果超过FLASH的存储空间，则报错返回
            FLASH_DataCacheCmd(ENABLE);
            FLASH_Lock();
            return -2;
        }
        p = p->link;
    }
    //写入结束标志
    str_index = 0;
    while(param_end[str_index] != 0){
        FLASH_ProgramByte(param_addr++,param_end[str_index++]);
    }
    FLASH_ProgramByte(param_addr++,0);   //字符串"end"结束标志
    FLASH_ProgramWord(param_addr,0);
    param_addr = param_addr+4;
    
    FLASH_DataCacheCmd(ENABLE);
    FLASH_Lock();  //  写保护
    if(param_addr > PARAM_FLASH_ADDR_END){   //如果超过FLASH的存储空间，则报错返回
        return -2;
    }
    return 1;
}
