#include <stdint.h> 

#include "stm32f4xx_gpio.h"


//siic

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
//用哪种EEPROM就写哪个型号咯~
#define EE_TYPE AT24C02
					  
uint8_t at24cxx_read_one_byte(uint16_t read_addr);							//指定地址读取一个字节
void at24cxx_write_one_byte(uint16_t write_addr, uint8_t data_to_write);		//指定地址写入一个字节
void at24cxx_write_len_byte(uint16_t write_addr, uint32_t data_to_write, uint8_t len);//指定地址开始写入指定长度的数据
uint32_t at24cxx_read_len_byte(uint16_t read_addr, uint8_t len);					//指定地址开始读取指定长度数据
void at24cxx_write(uint16_t write_addr, uint8_t *p_buffer, uint16_t num_to_write);	//从指定地址开始写入指定长度的数据
void at24cxx_read(uint16_t read_addr, uint8_t *p_buffer, uint16_t num_to_read);   	//从指定地址开始读出指定长度的数据

uint8_t at24cxx_check(void);  //检查器件
void at24cxx_config(void); //初始化IIC
