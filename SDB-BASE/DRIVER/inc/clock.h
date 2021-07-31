
#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "pindef.h"
#include "stm32f10x_conf.h"

typedef struct 
{
	unsigned short year;	
	unsigned char  month;
	unsigned char  day;
	unsigned char  week;
	unsigned char hour;	
	unsigned char min;  
	unsigned char sec;
}Calendar_obj;  

extern Calendar_obj Calendar;    

void RTC_Configuration(void);
void NVIC_Configuration(void);
void RTC_Init(void);
unsigned char Is_LeapYear(unsigned short  pyear);
unsigned char RTC_Set(Calendar_obj *Calendar_Info);
void Calendar_UI(Calendar_obj *Calendar_Info);
void RTC_Get(Calendar_obj *Calendar_Info);
void RTC_Thread(void);
#endif

