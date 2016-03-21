#include "stm32f4xx_gpio.h"

struct siic_pin {
	GPIO_TypeDef* port;
	uint16_t pin;
	uint8_t pin_num;
};

extern struct siic_pin siic_scl, siic_sda;

//IO方向设置
#define SDA_IN()  {siic_sda.port->MODER&=~(3<<(siic_sda.pin_num*2));\
				siic_sda.port->MODER|=0<<siic_sda.pin_num*2;}
#define SDA_OUT() {siic_sda.port->MODER&=~(3<<(siic_sda.pin_num*2));\
				siic_sda.port->MODER|=1<<siic_sda.pin_num*2;}
//IO操作函数	
#define READ_SDA   GPIO_ReadInputDataBit(siic_sda.port, siic_sda.pin)


void siic_config(void);                //初始化iic的IO口
void siic_start(void);				//发送iic开始信号
void siic_stop(void);	  			//发送siic停止信号
void siic_send_byte(uint8_t txd);		 //siic发送一个字节
uint8_t siic_read_byte(unsigned char ack);//siic读取一个字节
uint8_t siic_wait_ack(void); 			 //siic等待ACK信号
void siic_ack(void);						 //siic发送ACK信号
void siic_nack(void);					 //siic不发送ACK信号

void siic_write_bit(struct siic_pin siic_PIN, BitAction BitVal);
void siic_write_one_byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t siic_read_one_byte(uint8_t daddr,uint8_t addr);	
