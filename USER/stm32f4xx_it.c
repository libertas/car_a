/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

#include "clock.h"
#include "pwm.h"
#include "usart.h"
 

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_SendData(USART3, USART_ReceiveData(USART1));
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_SendData(USART3, USART_ReceiveData(USART2));
	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_SendData(USART3, USART_ReceiveData(USART3));
	}
}

void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_SendData(UART4, USART_ReceiveData(UART4));
	}
}


void TIM6_DAC_IRQHandler(void)
{
	static uint16_t t[PWM_CHANNEL_NUM] = {0};
	uint8_t i;
	
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		for(i = 0; i < PWM_CHANNEL_NUM; i++) {
			t[i]++;
			if(t[i] > PWMHighTime[i]) {
				if(t[i] < PWMTotal[i]) {
					GPIO_WriteBit(PWMPorts[i], PWMPins[i], Bit_RESET);
				} else {
					GPIO_WriteBit(PWMPorts[i], PWMPins[i], Bit_SET);
					t[i] = 0;
				}
			}
		}
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

#include "encoder.h"
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);

		g_rotary_fan += (TIM_GetCounter(TIM3)-4000);
		TIM3->CNT = 4000;
		
		g_rotary_x += (TIM_GetCounter(TIM4)-4000);
		TIM4->CNT = 4000;
		
		g_rotary_y += (TIM_GetCounter(TIM5)-4000);
		TIM5->CNT = 4000;
	} 
}


void EXTI0_IRQHandler(void)
{
	delay_ms(10);
	if(0 == GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0)) {
		printf("exti0 at pf0\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}


void EXTI1_IRQHandler(void)
{
	delay_ms(10);
	if(0 == GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_1)) {
		printf("exti1 at pf1\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}


void EXTI2_IRQHandler(void)
{
	delay_ms(10);
	if(0 == GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_2)) {
		printf("exti2 at pf2\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI15_10_IRQHandler(void)
{
	//if(SET == EXTI_GetITStatus(EXTI_Line11)){
		//EXTI_ClearFlag(EXTI_Line11);
		delay_ms(10);
		if(0 == GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_11)) {
			printf("exti11 at pf11\n");
			//delay_ms(10);
			//EXTI_ClearFlag(EXTI_Line11);
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
	//}
}  
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
