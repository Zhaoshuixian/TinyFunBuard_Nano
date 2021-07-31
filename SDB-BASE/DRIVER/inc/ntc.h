
#ifndef __NTC_H__
#define __NTC_H__

#include "stm32f10x_conf.h"
#include "bsp.h"

typedef struct
{
	
	const unsigned short adc_tabL[20];
	const unsigned short adc_tabH[60];	

}ntc_st;

extern sensor_st thermal;

void thermal_read(void);

#endif 

