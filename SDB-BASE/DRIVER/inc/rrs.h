
#ifndef __RRS_H__
#define __RRS_H__



#include "pindef.h"
#include "stm32f10x_conf.h"

#define RRS_Handler   TIM3_IRQHandler


#define TEMP_TASK_TIME      500   /*500ms*/
#define RTC_TASK_TIME       500  /*1000ms*/
#define TCP_TASK_TIME       10   /*10ms*/
#define LED_TASK_TIME       10
#define IR_TASK_TIME        10  /*60ms*/
#define OLED_TASK_TIME      10
#define MENU_TASK_TIME      10

typedef struct 
{
	unsigned char IsRun; 
	unsigned int  TaskTimer; 
	unsigned int  TaskTime;
	void (*TaskPointer)(void); 
}Task_Struct;


void ST_Task(void);//������

#endif


