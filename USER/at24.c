#include "stm32f4xx_gpio.h"

#include "at24.h"
#include "clock.h"
	
struct iic_pin IIC_SCL = {GPIOB, GPIO_Pin_8};
struct iic_pin IIC_SDA = {GPIOB, GPIO_Pin_9};


void iic_config(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

	//GPIOB8,B9��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	iic_write_bit(IIC_SCL, Bit_SET);
	iic_write_bit(IIC_SDA, Bit_SET);
}

void iic_start(void)
{
	SDA_OUT();     //sda�����
	iic_write_bit(IIC_SDA, Bit_SET);	  	  
	iic_write_bit(IIC_SCL, Bit_SET);
	delay_us(4);
 	iic_write_bit(IIC_SDA, Bit_RESET);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	iic_write_bit(IIC_SCL, Bit_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  

void iic_stop(void)
{
	SDA_OUT();//sda�����
	iic_write_bit(IIC_SCL, Bit_RESET);
	iic_write_bit(IIC_SDA, Bit_RESET);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	iic_write_bit(IIC_SCL, Bit_SET); 
	iic_write_bit(IIC_SDA, Bit_SET);//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t iic_wait_ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	iic_write_bit(IIC_SCL, Bit_RESET);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IICдbit
void iic_write_bit(struct iic_pin IIC_PIN, BitAction BitVal)
{
	GPIO_WriteBit(IIC_PIN.port, IIC_PIN.pin, BitVal);
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void iic_send_byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    iic_write_bit(IIC_SCL, Bit_RESET);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        iic_write_bit(IIC_SDA, (txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		iic_write_bit(IIC_SCL, Bit_SET);
		delay_us(2); 
		iic_write_bit(IIC_SCL, Bit_RESET);	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t iic_read_byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        iic_nack();//����nACK
    else
        iic_ack(); //����ACK   
    return receive;
}


//at24

//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
    iic_start();  
	if(EE_TYPE>AT24C16)
	{
		iic_send_byte(0XA0);	   //����д����
		iic_wait_ack();
		iic_send_byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}else iic_send_byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
	iic_wait_ack(); 
    iic_send_byte(ReadAddr%256);   //���͵͵�ַ
	iic_wait_ack();	    
	iic_start();  	 	   
	iic_send_byte(0XA1);           //�������ģʽ			   
	iic_wait_ack();	 
    temp=iic_read_byte(0);		   
    iic_stop();//����һ��ֹͣ����	    
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    iic_start();  
	if(EE_TYPE>AT24C16)
	{
		iic_send_byte(0XA0);	    //����д����
		iic_wait_ack();
		iic_send_byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}else iic_send_byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	iic_wait_ack();	   
    iic_send_byte(WriteAddr%256);   //���͵͵�ַ
	iic_wait_ack(); 	 										  		   
	iic_send_byte(DataToWrite);     //�����ֽ�							   
	iic_wait_ack();  		    	   
    iic_stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{  	
	uint8_t t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
