
#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f10x.h"

void xDelay_us(unsigned int Nus);
void xDelay_ms(unsigned int Nms);
void sDelay_us(unsigned int Kus);
void sDelay_ms(unsigned int Kms) ;
void Timer15_Init(void);

#endif
