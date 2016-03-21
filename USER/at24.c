#include "stm32f4xx_gpio.h"

#include "at24.h"
#include "clock.h"


struct siic_pin siic_scl = {GPIOB, GPIO_Pin_8, 8};
struct siic_pin siic_sda = {GPIOB, GPIO_Pin_9, 9};

//��ʼ��IIC
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
	while(READ_SDA)
	{
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
    for(t=0;t<8;t++)
    {              
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
        siic_nack();//����nACK
    else
        siic_ack(); //����ACK   
    return receive;
}

void siic_write_bit(struct siic_pin siic_PIN, BitAction BitVal)
{
	GPIO_WriteBit(siic_PIN.port, siic_PIN.pin, BitVal);
}


//at24

//��ʼ��siic�ӿ�
void at24cxx_config(void)
{
	siic_config();//iic��ʼ��
}
//��at24cxxָ����ַ����һ������
//read_addr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 at24cxx_read_one_byte(u16 read_addr)
{				  
	u8 temp=0;		  	    																 
    siic_start();  
	if(EE_TYPE>AT24C16)
	{
		siic_send_byte(0XA0);	   //����д����
		siic_wait_ack();
		siic_send_byte(read_addr>>8);//���͸ߵ�ַ	    
	}else siic_send_byte(0XA0+((read_addr/256)<<1));   //����������ַ0XA0,д���� 	   
	siic_wait_ack(); 
    siic_send_byte(read_addr%256);   //���͵͵�ַ
	siic_wait_ack();	    
	siic_start();  	 	   
	siic_send_byte(0XA1);           //�������ģʽ			   
	siic_wait_ack();	 
    temp = siic_read_byte(0);		   
    siic_stop();//����һ��ֹͣ����	    
	return temp;
}
//��at24cxxָ����ַд��һ������
//write_addr  :д�����ݵ�Ŀ�ĵ�ַ    
//data_to_write:Ҫд�������
void at24cxx_write_one_byte(u16 write_addr,u8 data_to_write)
{				   	  	    																 
    siic_start();  
	if(EE_TYPE>AT24C16)
	{
		siic_send_byte(0XA0);	    //����д����
		siic_wait_ack();
		siic_send_byte(write_addr>>8);//���͸ߵ�ַ	  
	}else siic_send_byte(0XA0+((write_addr/256)<<1));   //����������ַ0XA0,д���� 	 
	siic_wait_ack();	   
    siic_send_byte(write_addr%256);   //���͵͵�ַ
	siic_wait_ack(); 	 										  		   
	siic_send_byte(data_to_write);     //�����ֽ�							   
	siic_wait_ack();  		    	   
    siic_stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
//��at24cxx�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//write_addr  :��ʼд��ĵ�ַ  
//data_to_write:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void at24cxx_write_len_byte(u16 write_addr,u32 data_to_write,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		at24cxx_write_one_byte(write_addr+t,(data_to_write>>(8*t))&0xff);
	}												    
}

//��at24cxx�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���at24cxx�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 at24cxx_check(void)
{
	u8 temp;
	temp = at24cxx_read_one_byte(255);//����ÿ�ο�����дat24cxx			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		at24cxx_write_one_byte(255,0X55);
	    temp = at24cxx_read_one_byte(255);	  
		if(temp == 0X55)return 0;
	}
	return 1;											  
}

//��at24cxx�����ָ����ַ��ʼ����ָ������������
//read_addr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//p_buffer  :���������׵�ַ
//num_to_read:Ҫ�������ݵĸ���
void at24cxx_read(u16 read_addr,u8 *p_buffer,u16 num_to_read)
{
	while(num_to_read)
	{
		*p_buffer++=at24cxx_read_one_byte(read_addr++);	
		num_to_read--;
	}
}  
//��at24cxx�����ָ����ַ��ʼд��ָ������������
//write_addr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//p_buffer   :���������׵�ַ
//num_to_write:Ҫд�����ݵĸ���
void at24cxx_write(u16 write_addr,u8 *p_buffer,u16 num_to_write)
{
	while(num_to_write--)
	{
		at24cxx_write_one_byte(write_addr,*p_buffer);
		write_addr++;
		p_buffer++;
	}
}
