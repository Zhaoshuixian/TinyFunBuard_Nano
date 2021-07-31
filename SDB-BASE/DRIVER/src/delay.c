
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
 SysTick->LOAD = Kus*24;         //ʱ�����       
 SysTick->CTRL = 0x00000005;             //������������ͬʱ����ʹ��HCLKʱ��Դ��     
 while(!(SysTick->CTRL&0x00010000)); //�ȴ�������0  
 SysTick->CTRL = 0X00000004;           //�رռ����� 
 SysTick->VAL  = 0X00000000;          //��ռ�����      
} 


///////////////////////////////////////////////////////////////////////////
void sDelay_ms(unsigned int Kms) 
{  
 SysTick->LOAD = 1000*Kms*24;         //ʱ�����       
 SysTick->CTRL = 0x00000005;             //������������ͬʱ����ʹ��HCLKʱ��Դ��     
 while(!(SysTick->CTRL&0x00010000)); //�ȴ�������0  
 SysTick->CTRL = 0X00000004;           //�رռ����� 
 SysTick->VAL  = 0X00000000;          //��ռ�����      
} 










