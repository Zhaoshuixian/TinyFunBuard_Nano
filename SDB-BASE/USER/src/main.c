
/*
Ӳ��ƽ̨:STM32F103C8T6�Կ������԰�
��ɹ����У�
1.�¶Ȳ�����
2.�������
3.OLED����
4.RTC
5.
6.
*/
#include "i2c.h"
#include "spi2.h"
#include "delay.h"
#include "bsp.h"
#include "rrs.h"
#include "tim.h"
#include "at24cxx.h"
#include "beep.h"
//#include "usart.h"
#include "clock.h"
#include "slcd.h"
#include "led.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
void stm32f10x_rcc_init(void)//
{
	 RCC_DeInit(); 
	 RCC_HSEConfig(RCC_HSE_ON);
	 if(RCC_WaitForHSEStartUp() == SUCCESS)
	 {
		 RCC_HCLKConfig(RCC_SYSCLK_Div1);//Configures the AHB clock (HCLK).Fmax=72MHz
		 RCC_PCLK2Config(RCC_SYSCLK_Div1);//Configures the High Speed APB clock (PCLK2).Fmax=72MHz
		 RCC_PCLK1Config(RCC_SYSCLK_Div1);//Configures the Low Speed APB clock (PCLK1).Fmax=72MHz
														
		 FLASH_SetLatency(FLASH_Latency_2);
		 FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);													
		 RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//72M
		 RCC_PLLCmd(ENABLE);
		 RCC_ADCCLKConfig(RCC_PCLK2_Div6); //
		 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
					 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK );
		 while(RCC_GetSYSCLKSource() != 0x08){}		
		 RCC_ClockSecuritySystemCmd(ENABLE);                 
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1|RCC_APB2Periph_ADC1, ENABLE);
		 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//
		}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{  
  stm32f10x_rcc_init();
//TIM2_Init();//FOR BEEP
  tim3_init();//FOR RRS
//	USART1_Init();//FOR EXT USART
//	at24cxx_init();//FOR EEPROM
  infrade_init();//FOR INFRADE
  tim1_init();//FOR INFRADE
  rtc_init(); //RTC
  led_init();  //FOR DOUBLE LED
  ntc_init(); //FOR NTC TEMPERATURE	
  OLED_Init(); //FOR 0.96OLED
  while(1)
  {
		ST_Task();
		RT_Task();
  }
}

