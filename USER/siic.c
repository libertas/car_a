#include "stm32f4xx_gpio.h"

#include "clock.h"
#include "siic.h"

struct siic_pin siic_scl = {GPIOB, GPIO_Pin_8, 8};
struct siic_pin siic_sda = {GPIOB, GPIO_Pin_9, 9};


void siic_config(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

	//GPIOB8,B9初始化设置
	GPIO_InitStructure.GPIO_Pin = siic_scl.pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(siic_scl.port, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = siic_sda.pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(siic_sda.port, &GPIO_InitStructure);//初始化
	siic_write_bit(siic_scl, Bit_SET);
	siic_write_bit(siic_sda, Bit_SET);
}

void siic_start(void)
{
	SDA_OUT();     //sda线输出
	siic_write_bit(siic_sda, Bit_SET);	  	  
	siic_write_bit(siic_scl, Bit_SET);
	delay_us(4);
 	siic_write_bit(siic_sda, Bit_RESET);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	siic_write_bit(siic_scl, Bit_RESET);//钳住I2C总线，准备发送或接收数据 
}	  

void siic_stop(void)
{
	SDA_OUT();//sda线输出
	siic_write_bit(siic_scl, Bit_RESET);
	siic_write_bit(siic_sda, Bit_RESET);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	siic_write_bit(siic_scl, Bit_SET); 
	siic_write_bit(siic_sda, Bit_SET);//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 siic_wait_ack(void)
{
	u8 uc_err_time=0;
	SDA_IN();      //SDA设置为输入  
	siic_write_bit(siic_sda, Bit_SET);
	delay_us(1);	   
	siic_write_bit(siic_scl, Bit_SET);
	delay_us(1);	 
	while(READ_SDA) {
		uc_err_time++;
		if(uc_err_time>250)
		{
			siic_stop();
			return 1;
		}
	}
	siic_write_bit(siic_scl, Bit_RESET);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void siic_ack(void)
{
	siic_write_bit(siic_scl, Bit_RESET);
	SDA_OUT();
	siic_write_bit(siic_sda, Bit_RESET);
	delay_us(2);
	siic_write_bit(siic_scl, Bit_SET);
	delay_us(2);
	siic_write_bit(siic_scl, Bit_RESET);
}
//不产生ACK应答		    
void siic_nack(void)
{
	siic_write_bit(siic_scl, Bit_RESET);
	SDA_OUT();
	siic_write_bit(siic_sda, Bit_SET);
	delay_us(2);
	siic_write_bit(siic_scl, Bit_SET);
	delay_us(2);
	siic_write_bit(siic_scl, Bit_RESET);
}					 				     
//siic发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void siic_send_byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    siic_write_bit(siic_scl, Bit_RESET);//拉低时钟开始数据传输
    for(t=0;t<8;t++) {              
        if(!((txd&0x80)>>7)) siic_write_bit(siic_sda, Bit_RESET);
		else siic_write_bit(siic_sda, Bit_SET);
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		siic_write_bit(siic_scl, Bit_SET);
		delay_us(2); 
		siic_write_bit(siic_scl, Bit_RESET);	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 siic_read_byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ ) {
        siic_write_bit(siic_scl, Bit_RESET); 
        delay_us(2);
		siic_write_bit(siic_scl,Bit_SET);
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        siic_nack();//发送nACK
    else
        siic_ack(); //发送ACK   
    return receive;
}

void siic_write_bit(struct siic_pin siic_PIN, BitAction BitVal)
{
	GPIO_WriteBit(siic_PIN.port, siic_PIN.pin, BitVal);
}
