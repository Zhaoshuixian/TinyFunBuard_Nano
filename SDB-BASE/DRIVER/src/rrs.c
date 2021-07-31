
#include "rrs.h"
#include "bsp.h"
#include "tim.h"
#include "ntc.h"
#include "beep.h"
#include "ir.h"
#include "clock.h"
#include "led.h"
#include "menu.h"

/////////////////////////////////分时任务运行调度配置//////////////////////////////////////////
Task_Struct Tasks[]=
{
	{0,TEMP_TASK_TIME,TEMP_TASK_TIME,Temperature_Thread},	
	{0,LED_TASK_TIME,LED_TASK_TIME,LED_Thread},	
	{0,IR_TASK_TIME,IR_TASK_TIME,IR_Thread},
	{0,RTC_TASK_TIME,RTC_TASK_TIME,RTC_Thread},
	{0,MENU_TASK_TIME,MENU_TASK_TIME,Menu_Thread},
};

unsigned char TASK_NUM = sizeof(Tasks)/sizeof(Task_Struct);

/////////////////////////////////////分时任务调度//////////////////////////////////////
void ST_Task(void)//任务处理
{
  static unsigned char i;
	for (i=0; i<TASK_NUM ; i++) //
	{
		if(Tasks[i].IsRun) //任务执行标志
		{
      Tasks[i].TaskPointer();//执行当前任务
			Tasks[i].IsRun = 0;//清除标志
		}
	}
}

///////////////////////////////////系统任务调度时基////////////////////////////////////////
void RRS_Handler(void)
{
  static unsigned char i;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		  for(i=0;i<TASK_NUM;i++) //遍历任务
      {
				if(Tasks[i].TaskTimer)
				{
					Tasks[i].TaskTimer--;
					if(0==Tasks[i].TaskTimer) 
					{
						Tasks[i].IsRun = 0x01;
						Tasks[i].TaskTimer = Tasks[i].TaskTime;
					}
				}
			}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源		
   }
}

