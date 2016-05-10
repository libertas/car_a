#include "cmd_func.h"
#include "cmd.h"
#include "parameter.h"  //工程关联的，如果需要移植本代码，则去掉该头文件
#include "stm32f4xx.h"
#include "stdlib.h"
#include "string.h"
#include "usart.h"
#include "movement.h"



void cmd_param_func(int argc,char *argv[]){
    float param_value;
   // char param_name[PARAM_NAME_LENGTH];
    list_node *p;
    int group;
    if(argc < 2){
        uprintf(CMD_USARTx,"param <cmd>");
    }
    if(strcmp(argv[1],"print") == 0){
        param_print(param_group_now());
    }else if(strcmp(argv[1],"set") == 0){
        param_value = atof((char *)argv[3]);
        if(param_set(argv[2],param_value) < 0){
            uprintf(CMD_USARTx,"not found param name:%s",argv[2]);
        }
    }else if(strcmp(argv[1],"save") == 0){   //保存参数到芯片上
        param_save_to_flash();
    }else if(strcmp(argv[1],"switch") == 0){   //切换参数组
        param_switch(atoi(argv[2]));
    }else if(strcmp(argv[1],"mobile-print") == 0){
        group = atoi(argv[2]);
        p = get_param_list();
        p = p->link;
        while(p != NULL){
            uprintf(CMD_USARTx,"[P#%s#%4f]",p->data->param_name,p->data->param_value[group]);
            p = p->link;
        }
		}else if(strcmp(argv[1],"reset") == 0){
				param_list_reset();
		}
		else{
        uprintf(CMD_USARTx,"arg not found");
        return;
    }
}

void cmd_reboot_func(int argc,char *argv[]){
    NVIC_SystemReset();
}

void cmd_stop_func(int argc,char *argv[]){
		stop_all();
		uprintf(USART1, "s r0x2f 0\r\n\n");
}

void cmd_maxon_func(int argc,char *argv[]){
		if(argc < 2){
        uprintf(CMD_USARTx,"param <cmd>");
    }
		if(strcmp(argv[1], "baud_9600") == 0){
				uprintf(USART1,"s r0x90 9600\r\n\n");
				uart1_config(9600);
		}else if(strcmp(argv[1], "baud_115200") == 0){
				uprintf(USART1, "s r0x90 115200\r\n\n");
				uart1_config(115200);
		}else if(strcmp(argv[1], "up_1") == 0){
				uprintf(USART1, "s r0x2f 1000000\r\n\n");
		}else if(strcmp(argv[1], "up_2") == 0){
				uprintf(USART1, "s r0x2f 1500000\r\n\n");
		}else if(strcmp(argv[1], "up_3") == 0){
				uprintf(USART1, "s r0x2f 2000000\r\n\n");
		}else if(strcmp(argv[1], "down_1") == 0){
				uprintf(USART1, "s r0x2f -1000000\r\n\n");
		}else if(strcmp(argv[1], "down_2") == 0){
				uprintf(USART1, "s r0x2f -1500000\r\n\n");
		}else if(strcmp(argv[1], "down_3") == 0){
				uprintf(USART1, "s r0x2f -2000000\r\n\n");
		}
}

