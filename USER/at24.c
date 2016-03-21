#include "siic.h"
#include "at24.h"
#include "clock.h"


//��ʼ��siic�ӿ�
void at24cxx_config(void)
{
	siic_config();//iic��ʼ��
}
//��at24cxxָ����ַ����һ������
//read_addr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
uint8_t at24cxx_read_one_byte(uint16_t read_addr)
{				  
	uint8_t temp=0;		  	    																 
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
void at24cxx_write_one_byte(uint16_t write_addr,uint8_t data_to_write)
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
void at24cxx_write_len_byte(uint16_t write_addr,uint32_t data_to_write,uint8_t Len)
{  	
	uint8_t t;
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
uint32_t at24cxx_read_len_byte(uint16_t read_addr,uint8_t Len)
{  	
	uint8_t t;
	uint32_t temp=0;
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
uint8_t at24cxx_check(void)
{
	uint8_t temp;
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
void at24cxx_read(uint16_t read_addr,uint8_t *p_buffer,uint16_t num_to_read)
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
void at24cxx_write(uint16_t write_addr,uint8_t *p_buffer,uint16_t num_to_write)
{
	while(num_to_write--)
	{
		at24cxx_write_one_byte(write_addr,*p_buffer);
		write_addr++;
		p_buffer++;
	}
}
