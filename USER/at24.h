#include <stdint.h> 

#include "stm32f4xx_gpio.h"


//siic

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
//������EEPROM��д�ĸ��ͺſ�~
#define EE_TYPE AT24C02
					  
uint8_t at24cxx_read_one_byte(uint16_t read_addr);							//ָ����ַ��ȡһ���ֽ�
void at24cxx_write_one_byte(uint16_t write_addr, uint8_t data_to_write);		//ָ����ַд��һ���ֽ�
void at24cxx_write_len_byte(uint16_t write_addr, uint32_t data_to_write, uint8_t len);//ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t at24cxx_read_len_byte(uint16_t read_addr, uint8_t len);					//ָ����ַ��ʼ��ȡָ����������
void at24cxx_write(uint16_t write_addr, uint8_t *p_buffer, uint16_t num_to_write);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void at24cxx_read(uint16_t read_addr, uint8_t *p_buffer, uint16_t num_to_read);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

uint8_t at24cxx_check(void);  //�������
void at24cxx_config(void); //��ʼ��IIC
