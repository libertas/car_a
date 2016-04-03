#include "cmd_func.h"
#include "parameter.h"  //工程关联的，如果需要移植本代码，则去掉该头文件
#include "stm32f4xx.h"
#include "global.h"   //工程关联的，如果需要移植本代码，则去掉该头文件
#include "stdlib.h"
#include "string.h"

//void cmd_param_save(int argc,char *argv[]){
//    if(param_save_to_flash() == -1){
//        //lcd_show_string(10,100,200,100,16,"flash erase error!");
//    }
//}


//void cmd_param_print(int argc,char *argv[]){
//    list_print(UART5,&g_param_list,param_group);
//}


//void cmd_param_reset(int argc,char *argv[]){
//    param_list_reset();
//}


//void cmd_param_switch(int argc,char *argv[]){
//    param_switch(&g_control_param,param_group);
//}


void cmd_hello_func(int argc,char *argv[]){
	  uprintf(USART1,"just test hello\n");
}

void cmd_param_func(int argc,char *argv[]){
	  float param_value;
	  char param_name[PARAM_NAME_LENGTH];
	  if(argc < 2){
	      uprintf(USART1,"param <cmd>");
		}
	  if(strcmp(argv[1],"print") == 0){
			  param_print(param_group_now());
		}else if(strcmp(argv[1],"set") == 0){
			  param_value = atof((char *)argv[3]);
			  if(param_set(argv[2],param_value) < 0){
					  uprintf(PARAM_USARTx,"not found param name:%s",argv[2]);
				}
		}else if(strcmp(argv[1],"save") == 0){
			  param_save_to_flash();
	  }else{
			  uprintf(PARAM_USARTx,"arg not found");
			  return;
		}
}

void cmd_hello2_func(int argc,char *argv[]){
	  uprintf(USART1,"chuan shen ce shi d minling");
}


