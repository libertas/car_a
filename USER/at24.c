#include "stm32f4xx_gpio.h"

#include "at24.h"
#include "clock.h"


struct siic_pin siic_scl = {GPIOB, GPIO_Pin_8, 8};
struct siic_pin siic_sda = {GPIOB, GPIO_Pin_9, 9};

//初始化IIC
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
	while(READ_SDA)
	{
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
    for(t=0;t<8;t++)
    {              
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
    for(i=0;i<8;i++ )
	{
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


//at24

//初始化siic接口
void at24cxx_config(void)
{
	siic_config();//iic初始化
}
//在at24cxx指定地址读出一个数据
//read_addr:开始读数的地址  
//返回值  :读到的数据
u8 at24cxx_read_one_byte(u16 read_addr)
{				  
	u8 temp=0;		  	    																 
    siic_start();  
	if(EE_TYPE>AT24C16)
	{
		siic_send_byte(0XA0);	   //发送写命令
		siic_wait_ack();
		siic_send_byte(read_addr>>8);//发送高地址	    
	}else siic_send_byte(0XA0+((read_addr/256)<<1));   //发送器件地址0XA0,写数据 	   
	siic_wait_ack(); 
    siic_send_byte(read_addr%256);   //发送低地址
	siic_wait_ack();	    
	siic_start();  	 	   
	siic_send_byte(0XA1);           //进入接收模式			   
	siic_wait_ack();	 
    temp = siic_read_byte(0);		   
    siic_stop();//产生一个停止条件	    
	return temp;
}
//在at24cxx指定地址写入一个数据
//write_addr  :写入数据的目的地址    
//data_to_write:要写入的数据
void at24cxx_write_one_byte(u16 write_addr,u8 data_to_write)
{				   	  	    																 
    siic_start();  
	if(EE_TYPE>AT24C16)
	{
		siic_send_byte(0XA0);	    //发送写命令
		siic_wait_ack();
		siic_send_byte(write_addr>>8);//发送高地址	  
	}else siic_send_byte(0XA0+((write_addr/256)<<1));   //发送器件地址0XA0,写数据 	 
	siic_wait_ack();	   
    siic_send_byte(write_addr%256);   //发送低地址
	siic_wait_ack(); 	 										  		   
	siic_send_byte(data_to_write);     //发送字节							   
	siic_wait_ack();  		    	   
    siic_stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在at24cxx里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//write_addr  :开始写入的地址  
//data_to_write:数据数组首地址
//Len        :要写入数据的长度2,4
void at24cxx_write_len_byte(u16 write_addr,u32 data_to_write,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		at24cxx_write_one_byte(write_addr+t,(data_to_write>>(8*t))&0xff);
	}												    
}

//在at24cxx里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 at24cxx_read_len_byte(u16 read_addr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp += at24cxx_read_one_byte(read_addr+Len-t-1); 	 				   
	}
	return temp;												    
}
//检查at24cxx是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 at24cxx_check(void)
{
	u8 temp;
	temp = at24cxx_read_one_byte(255);//避免每次开机都写at24cxx			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		at24cxx_write_one_byte(255,0X55);
	    temp = at24cxx_read_one_byte(255);	  
		if(temp == 0X55)return 0;
	}
	return 1;											  
}

//在at24cxx里面的指定地址开始读出指定个数的数据
//read_addr :开始读出的地址 对24c02为0~255
//p_buffer  :数据数组首地址
//num_to_read:要读出数据的个数
void at24cxx_read(u16 read_addr,u8 *p_buffer,u16 num_to_read)
{
	while(num_to_read)
	{
		*p_buffer++=at24cxx_read_one_byte(read_addr++);	
		num_to_read--;
	}
}  
//在at24cxx里面的指定地址开始写入指定个数的数据
//write_addr :开始写入的地址 对24c02为0~255
//p_buffer   :数据数组首地址
//num_to_write:要写入数据的个数
void at24cxx_write(u16 write_addr,u8 *p_buffer,u16 num_to_write)
{
	while(num_to_write--)
	{
		at24cxx_write_one_byte(write_addr,*p_buffer);
		write_addr++;
		p_buffer++;
	}
}
