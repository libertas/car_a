#include "misc.h"

#include "clock.h"

static uint8_t  fac_us=0;							//us��ʱ������			   
static uint16_t fac_ms=0;							//ms��ʱ������


void systick_config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick_Config(SystemCoreClock / 1000000);
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

#if 0 
void delay_us(uint32_t us)
{
	uint32_t old_time = SysTick->VAL;
	uint32_t now_time;
	while(us) {
		now_time = SysTick->VAL;
		us -= old_time - now_time;
		old_time = now_time;
	}
}
#endif

void delay_us(uint32_t nus)
{		
	u32 temp;	    	 
	SysTick->LOAD = nus * fac_us; 					//ʱ�����	  		 
	SysTick->VAL = 0x00;        							//��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp = SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL = 0X00;      							 //��ռ�����	 
}
