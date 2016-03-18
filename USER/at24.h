#include <stdint.h> 

#include "stm32f4xx_gpio.h"

struct iic_pin {
	GPIO_TypeDef* port;
	uint16_t pin;
};

extern struct iic_pin IIC_SCL, IIC_SDA;

//IO方向设置
#define SDA_IN()  {IIC_SDA.port->MODER&=~(3<<(IIC_SDA.pin*2));IIC_SDA.port->MODER|=0<<IIC_SDA.pin*2;}//PB9输入模式
#define SDA_OUT() {IIC_SDA.port->MODER&=~(3<<(IIC_SDA.pin*2));IIC_SDA.port->MODER|=1<<IIC_SDA.pin*2;}//PB9输出模式
//IO操作函数
#define READ_SDA   GPIO_ReadInputDataBit(IIC_SDA.port, IIC_SDA.pin)//输入SDA

//IIC所有操作函数
void iic_config(void);              //初始化IIC的IO口				 
void iic_start(void);				//发送IIC开始信号
void iic_stop(void);	  			//发送IIC停止信号
void iic_send_byte(uint8_t txd);		 //IIC发送一个字节
uint8_t iic_read_byte(unsigned char ack);//IIC读取一个字节
uint8_t iic_wait_ack(void); 			 //IIC等待ACK信号
void iic_ack(void);					//IIC发送ACK信号
void iic_nack(void);				//IIC不发送ACK信号

void iic_write_bit(struct iic_pin, BitAction BitVal);
void iic_write_one_byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t iic_read_one_byte(uint8_t daddr,uint8_t addr);	  


//at24

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//用哪个型号就定义哪个型号咯
#define EE_TYPE AT24C256

					  
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);//指定地址开始写入指定长度的数据
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);   	//从指定地址开始读出指定长度的数据

uint8_t AT24CXX_Check(void);  //检查器件
