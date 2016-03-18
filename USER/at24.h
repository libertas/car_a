#include <stdint.h> 

#include "stm32f4xx_gpio.h"

struct iic_pin {
	GPIO_TypeDef* port;
	uint16_t pin;
};

extern struct iic_pin IIC_SCL, IIC_SDA;

//IO��������
#define SDA_IN()  {IIC_SDA.port->MODER&=~(3<<(IIC_SDA.pin*2));IIC_SDA.port->MODER|=0<<IIC_SDA.pin*2;}//PB9����ģʽ
#define SDA_OUT() {IIC_SDA.port->MODER&=~(3<<(IIC_SDA.pin*2));IIC_SDA.port->MODER|=1<<IIC_SDA.pin*2;}//PB9���ģʽ
//IO��������
#define READ_SDA   GPIO_ReadInputDataBit(IIC_SDA.port, IIC_SDA.pin)//����SDA

//IIC���в�������
void iic_config(void);              //��ʼ��IIC��IO��				 
void iic_start(void);				//����IIC��ʼ�ź�
void iic_stop(void);	  			//����IICֹͣ�ź�
void iic_send_byte(uint8_t txd);		 //IIC����һ���ֽ�
uint8_t iic_read_byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t iic_wait_ack(void); 			 //IIC�ȴ�ACK�ź�
void iic_ack(void);					//IIC����ACK�ź�
void iic_nack(void);				//IIC������ACK�ź�

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
//���ĸ��ͺžͶ����ĸ��ͺſ�
#define EE_TYPE AT24C256

					  
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);//ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

uint8_t AT24CXX_Check(void);  //�������
