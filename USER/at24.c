#include "stm32f4xx_gpio.h"

#include "at24.h"
#include "clock.h"
	
struct iic_pin IIC_SCL = {GPIOB, GPIO_Pin_8};
struct iic_pin IIC_SDA = {GPIOB, GPIO_Pin_9};


void iic_config(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

	//GPIOB8,B9初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	iic_write_bit(IIC_SCL, Bit_SET);
	iic_write_bit(IIC_SDA, Bit_SET);
}

void iic_start(void)
{
	SDA_OUT();     //sda线输出
	iic_write_bit(IIC_SDA, Bit_SET);	  	  
	iic_write_bit(IIC_SCL, Bit_SET);
	delay_us(4);
 	iic_write_bit(IIC_SDA, Bit_RESET);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	iic_write_bit(IIC_SCL, Bit_RESET);//钳住I2C总线，准备发送或接收数据 
}	  

void iic_stop(void)
{
	SDA_OUT();//sda线输出
	iic_write_bit(IIC_SCL, Bit_RESET);
	iic_write_bit(IIC_SDA, Bit_RESET);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	iic_write_bit(IIC_SCL, Bit_SET); 
	iic_write_bit(IIC_SDA, Bit_SET);//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t iic_wait_ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	iic_write_bit(IIC_SDA, Bit_SET);
	delay_us(1);	   
	iic_write_bit(IIC_SCL, Bit_SET);
	delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			iic_stop();
			return 1;
		}
	}
	iic_write_bit(IIC_SCL, Bit_RESET);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void iic_ack(void)
{
	iic_write_bit(IIC_SCL, Bit_RESET);
	SDA_OUT();
	iic_write_bit(IIC_SDA, Bit_RESET);
	delay_us(2);
	iic_write_bit(IIC_SCL, Bit_SET);
	delay_us(2);
	iic_write_bit(IIC_SCL, Bit_RESET);
}
//不产生ACK应答		    
void iic_nack(void)
{
	iic_write_bit(IIC_SCL, Bit_RESET);
	SDA_OUT();
	iic_write_bit(IIC_SDA, Bit_RESET);
	delay_us(2);
	iic_write_bit(IIC_SCL, Bit_SET);
	delay_us(2);
	iic_write_bit(IIC_SCL, Bit_RESET);
}					 				     
//IIC写bit
void iic_write_bit(struct iic_pin IIC_PIN, BitAction BitVal)
{
	GPIO_WriteBit(IIC_PIN.port, IIC_PIN.pin, BitVal);
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void iic_send_byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    iic_write_bit(IIC_SCL, Bit_RESET);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        iic_write_bit(IIC_SDA, (txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		iic_write_bit(IIC_SCL, Bit_SET);
		delay_us(2); 
		iic_write_bit(IIC_SCL, Bit_RESET);	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t iic_read_byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        iic_write_bit(IIC_SCL, Bit_RESET); 
        delay_us(2);
		iic_write_bit(IIC_SCL, Bit_SET);
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        iic_nack();//发送nACK
    else
        iic_ack(); //发送ACK   
    return receive;
}


//at24

//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
    iic_start();  
	if(EE_TYPE>AT24C16)
	{
		iic_send_byte(0XA0);	   //发送写命令
		iic_wait_ack();
		iic_send_byte(ReadAddr>>8);//发送高地址	    
	}else iic_send_byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	iic_wait_ack(); 
    iic_send_byte(ReadAddr%256);   //发送低地址
	iic_wait_ack();	    
	iic_start();  	 	   
	iic_send_byte(0XA1);           //进入接收模式			   
	iic_wait_ack();	 
    temp=iic_read_byte(0);		   
    iic_stop();//产生一个停止条件	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    iic_start();  
	if(EE_TYPE>AT24C16)
	{
		iic_send_byte(0XA0);	    //发送写命令
		iic_wait_ack();
		iic_send_byte(WriteAddr>>8);//发送高地址	  
	}else iic_send_byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	iic_wait_ack();	   
    iic_send_byte(WriteAddr%256);   //发送低地址
	iic_wait_ack(); 	 										  		   
	iic_send_byte(DataToWrite);     //发送字节							   
	iic_wait_ack();  		    	   
    iic_stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{  	
	uint8_t t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
{  	
	uint8_t t;
	uint32_t temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
