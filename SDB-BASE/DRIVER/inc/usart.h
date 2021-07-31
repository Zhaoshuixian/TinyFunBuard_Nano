
#ifndef _USART_H_
#define _USART_H_

#include "pindef.h"
#include "stm32f10x.h"
#include <stdio.h>

void USART1_Init(void);
void USART_PayByte(unsigned char ByteData);//打印字节
void USART_PayMultiByte(unsigned char *nByteData,unsigned char len);//打印多字节
void USART_PayString(char *StrData);//打印字符串

#endif 
