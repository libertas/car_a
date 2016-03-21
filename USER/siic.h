#include "stm32f4xx_gpio.h"

struct siic_pin {
	GPIO_TypeDef* port;
	uint16_t pin;
	uint8_t pin_num;
};

extern struct siic_pin siic_scl, siic_sda;

//IO��������
#define SDA_IN()  {siic_sda.port->MODER&=~(3<<(siic_sda.pin_num*2));\
				siic_sda.port->MODER|=0<<siic_sda.pin_num*2;}
#define SDA_OUT() {siic_sda.port->MODER&=~(3<<(siic_sda.pin_num*2));\
				siic_sda.port->MODER|=1<<siic_sda.pin_num*2;}
//IO��������	
#define READ_SDA   GPIO_ReadInputDataBit(siic_sda.port, siic_sda.pin)


void siic_config(void);                //��ʼ��iic��IO��
void siic_start(void);				//����iic��ʼ�ź�
void siic_stop(void);	  			//����siicֹͣ�ź�
void siic_send_byte(uint8_t txd);		 //siic����һ���ֽ�
uint8_t siic_read_byte(unsigned char ack);//siic��ȡһ���ֽ�
uint8_t siic_wait_ack(void); 			 //siic�ȴ�ACK�ź�
void siic_ack(void);						 //siic����ACK�ź�
void siic_nack(void);					 //siic������ACK�ź�

void siic_write_bit(struct siic_pin siic_PIN, BitAction BitVal);
void siic_write_one_byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t siic_read_one_byte(uint8_t daddr,uint8_t addr);	
