

#include "clock.h"
#include "bsp.h"
#include "delay.h"
#include "string.h"
#include <stdio.h>
#include "slcd.h"

const char* wk_p[7]={"Mon","Tus","Wed","Thr","Fri","Sat","Sun"};
const unsigned char mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//平均的月份日期表
Calendar_obj Calendar=
{
   2019,05,29,3,19,29,0
};    //时钟结构体

/////////////////////////////////////////////////////////////////////////////////////////////
void RTC_Configuration(void)
{
   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);/* 使能PWR和BKP时钟 */  
    PWR_BackupAccessCmd(ENABLE); /* 使能对后备寄存器的访问 */     
    BKP_DeInit();/* 复位BKP寄存器 */    
    RCC_LSEConfig(RCC_LSE_ON);/* 使能LSE */    
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {}/*等待启动完成 */  
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);/* 将 RTC时钟设置为LSE这个32.768KHZ的晶振*/ 
    RCC_RTCCLKCmd(ENABLE); /* 使能RTC Clock */ 
    RTC_WaitForSynchro();/* 等待同步 */   
    RTC_WaitForLastTask();/* 等待对RTC寄存器最后的写操作完成*/  
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)*//* 配置了预分频值: 设置RTC时钟周期为1s */    
    RTC_WaitForLastTask();/* 等待对RTC寄存器最后的写操作完成 */    
//    RTC_ITConfig(RTC_IT_SEC, ENABLE);/* 使能RTC秒中断 */     
    RTC_WaitForLastTask();/* 等待对RTC寄存器最后的写操作完成 */  

}
/////////////////////////////////////////////////////////////////////////////////////////////
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void RTC_Init(void)
{
    /*如果是第一次配置时钟，则执行RCC_Configuration()进行配置*/
    if(BKP_ReadBackupRegister(BKP_DR1)!=0x1016)
    {
				RTC_Configuration();
				RTC_Set(&Calendar);
				BKP_WriteBackupRegister(BKP_DR1, 0x1016);//向执行的后备寄存器中写入用户程序数据
    }
    else
    {
        RTC_WaitForSynchro();//等待RTC寄存器同步完成
//        RTC_ITConfig(RTC_IT_SEC, ENABLE);//使能RTC秒中断
        RTC_WaitForLastTask();//等待最近一次对RTC寄存器的写操作完成
    }
    NVIC_Configuration();
    RTC_Get(&Calendar);//更新时间
}
/////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Is_LeapYear(unsigned short  pyear)
{
   unsigned char Leap_Flag=0;
	((((pyear%4)==0)&&(pyear%100)!=0)||(pyear%400==0))?(Leap_Flag=1):(Leap_Flag=0);
	
  return Leap_Flag;
}
/*
*设置时钟
*把输入的时钟转换为秒钟
*以1970年1月1日为基准
*1970~2099年为合法年份
返回值：0，成功；其它：错误
*/
/////////////////////////////////////////////////////////////////////////////////////////////
unsigned char RTC_Set(Calendar_obj *Calendar_Info)
{
    unsigned short t;
    unsigned int secCount=0;
    if(Calendar_Info->year<1970||Calendar_Info->year>2099)  
		{
			return 1;
		}
    for(t=1970;t<Calendar_Info->year;t++)    //把所有年份的秒钟相加
    {
			(Is_LeapYear(t))?(secCount+=31622400):(secCount+=31536000);////闰年的秒钟数
    }
    Calendar_Info->month-=1;//先减掉一个月再算秒数（如现在是5月10日，则只需要算前4个月的天数，再加上10天，然后计算秒数）
    for(t=0;t<Calendar_Info->month;t++)
    {
			secCount+=(unsigned int)mon_table[t]*86400;//月份秒钟数相加
			(Is_LeapYear(Calendar_Info->year)&&t==1)?(secCount+=86400):(0);//闰年，2月份增加一天的秒钟数
    }
    secCount+=(unsigned int)(Calendar_Info->day-1)*86400;//把前面日期的秒钟数相加（这一天还没过完，所以-1）
    secCount+=(unsigned int)Calendar_Info->hour*3600;//小时秒钟数
    secCount+=(unsigned int)Calendar_Info->min*60;//分钟秒钟数
    secCount+=Calendar_Info->sec;
    RTC_SetCounter(secCount);//设置RTC计数器的值
    RTC_WaitForLastTask();    //等待最近一次对RTC寄存器的写操作完成
    RTC_Get(&Calendar);//更新时间	
		return 0;
}

/*
得到当前的时间
成功返回0，错误返回其它
*/
/////////////////////////////////////////////////////////////////////////////////////////////
void RTC_Get(Calendar_obj *Calendar_Info) 
{
		static unsigned short dayCount=0;
		unsigned int secCount=0;
		unsigned int tmp=0;
		unsigned short tmp1=0;
		secCount=RTC_GetCounter();
		tmp=secCount/86400;//得到天数
		if(dayCount!=tmp)//超过一天
		{
				dayCount=tmp;
				tmp1=1970;//从1970年开始
				while(tmp>=365)
				{
						if(Is_LeapYear(tmp1))//是闰年
						{
							if(tmp>=366)
							{
								tmp-=366;
							}
							else
							{
								break; //减掉闰年的天数
							}								
						}
						else
						{
							 tmp-=365;//平年
						}
						tmp1++;
				}
				Calendar_Info->year=tmp1;//得到年份
				tmp1=0;
				while(tmp>=28)//超过一个月
				{
						if(Is_LeapYear(Calendar_Info->year)&&tmp1==1)//当年是闰年且轮循到2月
						{
								if(tmp>=29)
								{									
										tmp-=29;
								}
								else
								{
										break;
								}
						}
						else
						{
								if(tmp>=mon_table[tmp1])//平年
								{
										tmp-=mon_table[tmp1];
								}
								else
								{
										break;
								}
						}
						tmp1++;
				}
				Calendar_Info->month=tmp1+1;//得到月份，tmp1=0表示1月，所以要加1
				Calendar_Info->day=tmp+1;    //得到日期，因为这一天还没过完，所以tmp只到其前一天，但是显示的时候要显示正常日期
		}
		tmp=secCount%86400;//得到秒钟数
		Calendar_Info->hour=tmp/3600;//小时
		Calendar_Info->min=(tmp%3600)/60;//分钟
		Calendar_Info->sec=(tmp%3600)%60;//秒
}
/*
RTC时钟中断
每秒触发一次
*/
//void RTC_IRQHandler(void)
//{                                
//    if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
//    {
//        RTC_ClearITPendingBit(RTC_IT_ALR);//清闹钟中断        
//    }                                                    
//    RTC_ClearITPendingBit(RTC_IT_OW);//清闹钟中断
//    RTC_WaitForLastTask();                                                   
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
void RTC_Thread(void)
{
	RTC_Get(&Calendar);//时间获取
//	Calendar_UI(&Calendar);	      //调用日历时间UI
}








