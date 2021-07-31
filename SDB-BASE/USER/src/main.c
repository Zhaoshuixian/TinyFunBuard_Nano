
/*
硬件平台:STM32F103C8T6自开发测试板
完成功能有：
1.温度测量，
2.红外解码
3.OLED驱动
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
void STM32F103_RCC_Init(void)//STM32f100C8T6主频最大为24MHz
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
		 RCC_ADCCLKConfig(RCC_PCLK2_Div6); //ADC最大时钟不超过14M
		 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
					 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK );
		 while(RCC_GetSYSCLKSource() != 0x08){}		
		 RCC_ClockSecuritySystemCmd(ENABLE);                 //使能系统安全时钟 
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1|RCC_APB2Periph_ADC1, ENABLE);
		 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//开DMA设备时钟
		}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void SysInit(void)
{
 STM32F103_RCC_Init();
//TIM2_Init();//FOR BEEP
	TIM3_Init();//FOR RRS
//	USART1_Init();//FOR EXT USART
//	AT24CXX_Init();//FOR EEPROM
  IR_Init();//FOR INFRADE
	TIM1_Init();//FOR INFRADE
	RTC_Init(); //RTC
  LED_Init();  //FOR DOUBLE LED
	NTC_Init(); //FOR NTC TEMPERATURE	
	OLED_Init(); //FOR 0.96OLED

}

//////////////////////////////////////////////////////////////////////////////////////////////////
void RT_Task(void)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{  
  SysInit();
  while(1)
  {
		ST_Task();
		RT_Task();
  }
}

