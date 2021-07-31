
#ifndef __NTC_H__
#define __NTC_H__

#include "stm32f10x_conf.h"
#include "bsp.h"

typedef struct
{
	
	const unsigned short adc_tabL[20];
	const unsigned short adc_tabH[60];	

}ntc_st;

extern Environ_sensor Temperature;

unsigned char Find_Tab(unsigned short int cData,const unsigned short int  pTab[],unsigned char pTab_len);// �β�:��ǰ���� | ������ | ����
void Averg_Val(unsigned short int base_val,unsigned char n);
void Temperature_Get(Environ_sensor *Temperature_Info);//����¶�����
void Temperature_UI(Environ_sensor *Temperature_Info);
void Temperature_Thread(void);

#endif 

