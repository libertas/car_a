#include "cmd.h"
#include "stm32f4xx.h"

static cmd_struct cmd_tbl[] = {
    /*
     * 若需添加命令，需要在此加上：
     * CMD_ADD("命令名","命令信息",对应命令的执行函数名)
     * 注意最后一个不需要逗号，前面的都需要逗号
    */
    CMD_ADD("hello","just test",cmd_hello_func)
};

static char cmd_line[MAX_CMD_LINE_LENGTH];
static char cmd_argv[MAX_ARGC][MAX_CMD_ARG_LENGTH + 1];



void cmd_init(){
    Usart_InitTypeDef usart_init_stru;
    NVIC_InitStructure nvic_init_stru;
#if CMD_USARTx = USART1
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	
    GPIO_Configuration(GPIO_Pin_6,GPIO_Mode_AF, GPIO_Speed_50MHz,GPIO_OType_PP,GPIO_PuPd_NOPULL,GPIOB);   //USART1 TX
	GPIO_Configuration(GPIO_Pin_7,GPIO_Mode_AF, GPIO_Speed_50MHz,GPIO_OType_PP,GPIO_PuPd_NOPULL,GPIOB); //USART1 RX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);   
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);  
    nvic_init_stru.NVIC_IRQChannel = USART1_IRQn;
#elif CMD_USARTx = USART2
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	GPIO_Configuration(GPIO_Pin_2,GPIO_Mode_AF, GPIO_Speed_50MHz,GPIO_OType_PP,GPIO_PuPd_NOPULL,GPIOA);   //USART2 TX
	GPIO_Configuration(GPIO_Pin_3,GPIO_Mode_AF, GPIO_Speed_50MHz,GPIO_OType_PP,GPIO_PuPd_NOPULL,GPIOA); //USART2 RX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);   
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  
    nvic_init_stru.NVIC_IRQChannel = USART2_IRQn;
#elif CMD_USARTx = USART3
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    GPIO_Configuration(GPIO_Pin_10,GPIO_Mode_AF, GPIO_Speed_100MHz,GPIO_OType_PP,GPIO_PuPd_NOPULL,GPIOB);   //USART3 TX
	GPIO_Configuration(GPIO_Pin_11,GPIO_Mode_AF, GPIO_Speed_100MHz,GPIO_OType_PP,GPIO_PuPd_NOPULL,GPIOB); //USART3 RX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);   
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); 
    nvic_init_stru.NVIC_IRQChannel = USART3_IRQn;
#endif
    usart_init_stru.USART_BaudRate = CMD_USART_BAUD; 	   //设置波特率
	usart_init_stru.USART_WordLength = USART_WordLength_8b;    //一个帧中传输的数据位数（字长为8位数据格式）
	usart_init_stru.USART_StopBits = USART_StopBits_1; 	   //一个停止位
	usart_init_stru.USART_Parity = USART_Parity_No; 	//无奇偶校验
	usart_init_stru.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	usart_init_stru.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送和接受模式
	USART_Init(CMD_USARTx, &usart_init_stru);	 //初始化串口	
	USART_Cmd(CMD_USARTx, ENABLE);	  //使能串口
	USART_ITConfig(CMD_USARTx,USART_IT_RXNE, ENABLE);
    NVIC_PriorityGroupConfig(CMD_NVIC_GROUP);
    nvic_init_stru.NVIC_IRQChannelPreemptionPriority = CMD_NVIC_IRQPP;
    nvic_init_stru.NVIC_IRQChannelSubPriority = CMD_NVIC_IRQSP;
    nvic_init_stru.NVIC_IRQChannelCmd = ENABLE;	
    NVIC_Init(&nvic_init_stru);
}



int cmd_parse(char *cmd_line,int *argc,char *argv[]){
    char c_temp;
    char str_temp[MAX_CMD_ARG_LENGTH];
    int i = 0,arg_index = 0;

    *argc = 0;
    while((c_temp = cmd_line[i++]) != '\r'){
        if(c_temp == ' '){
            if(arg_index == 0 ){   //如果命令或者参数字符串第一个是空格，则忽略   
                continue;
            }
            //空格为参数或者命令的分隔符
            if(*argc == MAX_ARGC){   //如果参数个数过多,则返回
                return -1;
            }
            argv[*argc][arg_index] = 0;
            (*argc)++;
            arg_index = 0;
            continue;
        }
        if(arg_index == MAX_CMD_ARG_LENGTH){   //如果参数长度过长，则报错返回
            return -2;
        }
        argv[*argc][arg_index++] = c_temp;
    }
    if(argc == 0 && arg_index == 0){  //如果命令或者参数是空的，则返回
        return -3;
    }
    //最后一个参数的结束没有在上面的while循环中解析到
    argv[(*argc)++][arg_index] = 0;
    return 0;
}

int cmd_exec(int argc,char *argv[]){
    int cmd_index = 0;
    u32 cmd_num;
 
    cmd_num = sizeof(cmd_tbl)/sizeof(cmd_tbl[0]);

    if(argc == 0){  //如果参数是空的，则返回
        return -1;
    }
    for(cmd_index = 0;cmd_index < cmd_num;cmd_index++){   //查找命令
        if(strcmp(cmd_tbl[cmd_index].cmd_name,argv[0]) == 0){  //如果找到了命令，则执行命令相对应的函数
            cmd_tbl[cmd_index].cmd_func(acgc,argv);
            return 0;
        }
    }
    return -2;
}


#if CMD_USARTx = USART1
USART1_IRQHandler(void){
#elif CMD_USARTx = USART2
USART2_IRQHandler(void){
#elif CMD_USARTx = USART3
USART3_IRQHandler(void){
#endif 
    static u32 cmd_line_index = 0;
    int cmd_argc;
    int erro_n;
    u8 c_recv;
    
    if(USART_GetITStatus(CMD_USARTx,USART_IT_RXNE) != RESET){
        USART_ClearITPendingBit(CMD_USARTx,USART_IT_RXNE);
        c_recv = USART_ReceiveData(CMD_USARTx);
        if(c_recv == '\r'){  //接受完一次指令
            erro_n = cmd_parse(cmd_line,&cmd_argc,cmd_argv);  //解析命令
            if(erro_n < 0){
                //打印函数执行错误信息
                cmd_line_index = 0;
                return;
            }
            eero_n = cmd_exec(cmd_argc,cmd_argv);   //执行命令
            if(erro_n < 0){
                //打印函数执行错误信息
                cmd_line_index = 0;
                return;
            }
            cmd_line_index = 0;
            USART_SendData(CMD_USARTx,'\n');
            USART_SendData(CMD_USARTx,'\r');
            USART_SendData(CMD_USARTx,'>');
        }else{
            if(cmd_line_index == MAX_CMD_LINE_LENGTH){
                //打印命令行太长的信息
                cmd_line_index = 0;
                return;
            }
            cmd_line[cmd_line_index++] = (char)c_recv;
            USART_SendData(CMD_USARTx,c_recv);
        }
    }
}

