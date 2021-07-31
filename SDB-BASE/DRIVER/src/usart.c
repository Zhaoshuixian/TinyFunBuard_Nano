
#include "usart.h"

////////////////////////////////////////////////////////////////////////////////
void USART1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin =  USER_USART_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USER_USART_PORT, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = USER_USART_RX_PIN;
  GPIO_Init(USER_USART_PORT, &GPIO_InitStructure);	
	
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_ITConfig(USER_USART_NUM,USART_IT_RXNE, ENABLE); //Enable rx enable, 
  USART_Init(USER_USART_NUM,&USART_InitStructure);
  USART_Cmd(USER_USART_NUM,ENABLE);	
	
}

////////////////////////////////////////////////////////////////////////////////////////////
void USART_PayByte(unsigned char ByteData)//打印字节
{
    USART_SendData(USART1,ByteData);
    while(USART_GetFlagStatus(USER_USART_NUM,USART_FLAG_TXE) == RESET );
}

////////////////////////////////////////////////////////////////////////////////////////////
void USART_PayMultiByte(unsigned char *nByteData,unsigned char len)//打印多字节
{
   while(len--)
   {	
			USART_SendData(USART1,*nByteData++);
			while(USART_GetFlagStatus(USER_USART_NUM,USART_FLAG_TXE) == RESET );
   }		 
}

////////////////////////////////////////////////////////////////////////////////////////////
void USART_PayString(char *StrData)//打印字符串
{
   while(*StrData)
   {
      USART_SendData(USER_USART_NUM,*StrData++);
      while(USART_GetFlagStatus(USER_USART_NUM,USART_FLAG_TXE) == RESET);
   }
}

////////////////////////////////////////////////////////////////////////////////////////////
void USER_USART_ISR(void)//中断处理
{
	
	
}

/*********************************END OF FILE**********************************/

