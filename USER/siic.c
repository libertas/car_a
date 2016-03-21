#include "stm32f4xx_gpio.h"

#include "clock.h"
#include "siic.h"

struct siic_pin siic_scl = {GPIOB, GPIO_Pin_8, 8};
struct siic_pin siic_sda = {GPIOB, GPIO_Pin_9, 9};


void siic_config(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

	//GPIOB8,B9��ʼ������
	GPIO_InitStructure.GPIO_Pin = siic_scl.pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(siic_scl.port, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = siic_sda.pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(siic_sda.port, &GPIO_InitStructure);//��ʼ��
	siic_write_bit(siic_scl, Bit_SET);
	siic_write_bit(siic_sda, Bit_SET);
}

void siic_start(void)
{
	SDA_OUT();     //sda�����
	siic_write_bit(siic_sda, Bit_SET);	  	  
	siic_write_bit(siic_scl, Bit_SET);
	delay_us(4);
 	siic_write_bit(siic_sda, Bit_RESET);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	siic_write_bit(siic_scl, Bit_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  

void siic_stop(void)
{
	SDA_OUT();//sda�����
	siic_write_bit(siic_scl, Bit_RESET);
	siic_write_bit(siic_sda, Bit_RESET);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	siic_write_bit(siic_scl, Bit_SET); 
	siic_write_bit(siic_sda, Bit_SET);//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 siic_wait_ack(void)
{
	u8 uc_err_time=0;
	SDA_IN();      //SDA����Ϊ����  
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
	siic_write_bit(siic_scl, Bit_RESET);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//siic����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void siic_send_byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    siic_write_bit(siic_scl, Bit_RESET);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++) {              
        if(!((txd&0x80)>>7)) siic_write_bit(siic_sda, Bit_RESET);
		else siic_write_bit(siic_sda, Bit_SET);
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		siic_write_bit(siic_scl, Bit_SET);
		delay_us(2); 
		siic_write_bit(siic_scl, Bit_RESET);	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 siic_read_byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ ) {
        siic_write_bit(siic_scl, Bit_RESET); 
        delay_us(2);
		siic_write_bit(siic_scl,Bit_SET);
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        siic_nack();//����nACK
    else
        siic_ack(); //����ACK   
    return receive;
}

void siic_write_bit(struct siic_pin siic_PIN, BitAction BitVal)
{
	GPIO_WriteBit(siic_PIN.port, siic_PIN.pin, BitVal);
}
