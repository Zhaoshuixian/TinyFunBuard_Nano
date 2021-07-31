
#include "delay.h"



///////////////////////////////////////////////////////////////////////////
void xDelay_us(unsigned int Nus)
{
	 unsigned char i;
	 unsigned int j;
	for(i=0;i<2;i++)//
	for(j=0;j<Nus;j++);
}

///////////////////////////////////////////////////////////////////////////
void xDelay_ms(unsigned int Nms)
{
//	 unsigned char i;
	 unsigned int j;
//	for(i=0;i<1;i++)//
	for(j=0;j<1980*Nms;j++);
}


///////////////////////////////////////////////////////////////////////////
void sDelay_us(unsigned int Kus) 
{  
 SysTick->LOAD = Kus*24;         //时间加载       
 SysTick->CTRL = 0x00000005;             //开启计数器（同时设置使用HCLK时钟源）     
 while(!(SysTick->CTRL&0x00010000)); //等待倒数至0  
 SysTick->CTRL = 0X00000004;           //关闭计数器 
 SysTick->VAL  = 0X00000000;          //清空计数器      
} 


///////////////////////////////////////////////////////////////////////////
void sDelay_ms(unsigned int Kms) 
{  
 SysTick->LOAD = 1000*Kms*24;         //时间加载       
 SysTick->CTRL = 0x00000005;             //开启计数器（同时设置使用HCLK时钟源）     
 while(!(SysTick->CTRL&0x00010000)); //等待倒数至0  
 SysTick->CTRL = 0X00000004;           //关闭计数器 
 SysTick->VAL  = 0X00000000;          //清空计数器      
} 










