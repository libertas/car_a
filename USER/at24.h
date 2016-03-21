#include <stdint.h> 


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
