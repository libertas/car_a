#include "misc.h"

#include "clock.h"

static uint16_t fac_us = 0;			   

void system_clk_set(void)
{ 
    ErrorStatus HSEStartUpStatus;
  RCC_DeInit();
 
  RCC_HSEConfig(RCC_HSE_ON );   //���ⲿʱ��
 
  HSEStartUpStatus = RCC_WaitForHSEStartUp();  //�ȴ��ⲿʱ�Ӵ����ȶ�
 
  if(HSEStartUpStatus == SUCCESS)     
  {
    FLASH_SetLatency(FLASH_Latency_5);   
    FLASH_PrefetchBufferCmd(ENABLE);       //flashʱ�ӵ��������  
		RCC_PLLCmd(DISABLE);  //����PLL֮ǰ��Ҫ�ر�PLL
		RCC_HCLKConfig(RCC_SYSCLK_Div1);   //HCLK��Ƶ
    RCC_PCLK2Config(RCC_HCLK_Div1);   //PCLK2��Ƶ
		RCC_PCLK1Config(RCC_HCLK_Div1);    //PCLK1��Ƶ
    RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7);    //sysclk = 168MHZ  ,,���㹫ʽ�μ������ֲ�
    RCC_PLLCmd(ENABLE); //ʹ��PLL
 
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)      //�ȴ�����ֱ��PLLʹ�����
       {
       }
 
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  //ѡ��PLLʱ��Ϊϵͳʱ��
 
    while(RCC_GetSYSCLKSource() != 0x08)       //�ȴ�����PLLʱ�����ó�ϵͳʱ�����
       { 
       }
     }
    #if(__FPU_PRESENT == 1)&&(__FPU_USED == 1)
			SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  //����FPU
		#endif

}

void systick_config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SystemCoreClock / 1000000 / 8;	

	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void delay_us(uint32_t us)
{		
	u32 tmp;

	SysTick->LOAD = us * fac_us;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;

	do {
		tmp = SysTick->CTRL;
	} while((tmp & 0x01) && !(tmp & (1<<16)));

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0X00;
}

void delay_ms(uint32_t ms)
{
	while(ms--)
		delay_us(1000);
}
