

#include "clock.h"
#include "bsp.h"
#include "delay.h"
#include "string.h"
#include <stdio.h>
#include "slcd.h"

const char* wk_p[7]={"Mon","Tus","Wed","Thr","Fri","Sat","Sun"};
const unsigned char mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//ƽ�����·����ڱ�
Calendar_obj Calendar=
{
   2019,05,29,3,19,29,0
};    //ʱ�ӽṹ��

/////////////////////////////////////////////////////////////////////////////////////////////
void RTC_Configuration(void)
{
   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);/* ʹ��PWR��BKPʱ�� */  
    PWR_BackupAccessCmd(ENABLE); /* ʹ�ܶԺ󱸼Ĵ����ķ��� */     
    BKP_DeInit();/* ��λBKP�Ĵ��� */    
    RCC_LSEConfig(RCC_LSE_ON);/* ʹ��LSE */    
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {}/*�ȴ�������� */  
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);/* �� RTCʱ������ΪLSE���32.768KHZ�ľ���*/ 
    RCC_RTCCLKCmd(ENABLE); /* ʹ��RTC Clock */ 
    RTC_WaitForSynchro();/* �ȴ�ͬ�� */   
    RTC_WaitForLastTask();/* �ȴ���RTC�Ĵ�������д�������*/  
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)*//* ������Ԥ��Ƶֵ: ����RTCʱ������Ϊ1s */    
    RTC_WaitForLastTask();/* �ȴ���RTC�Ĵ�������д������� */    
//    RTC_ITConfig(RTC_IT_SEC, ENABLE);/* ʹ��RTC���ж� */     
    RTC_WaitForLastTask();/* �ȴ���RTC�Ĵ�������д������� */  

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
    /*����ǵ�һ������ʱ�ӣ���ִ��RCC_Configuration()��������*/
    if(BKP_ReadBackupRegister(BKP_DR1)!=0x1016)
    {
				RTC_Configuration();
				RTC_Set(&Calendar);
				BKP_WriteBackupRegister(BKP_DR1, 0x1016);//��ִ�еĺ󱸼Ĵ�����д���û���������
    }
    else
    {
        RTC_WaitForSynchro();//�ȴ�RTC�Ĵ���ͬ�����
//        RTC_ITConfig(RTC_IT_SEC, ENABLE);//ʹ��RTC���ж�
        RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
    }
    NVIC_Configuration();
    RTC_Get(&Calendar);//����ʱ��
}
/////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Is_LeapYear(unsigned short  pyear)
{
   unsigned char Leap_Flag=0;
	((((pyear%4)==0)&&(pyear%100)!=0)||(pyear%400==0))?(Leap_Flag=1):(Leap_Flag=0);
	
  return Leap_Flag;
}
/*
*����ʱ��
*�������ʱ��ת��Ϊ����
*��1970��1��1��Ϊ��׼
*1970~2099��Ϊ�Ϸ����
����ֵ��0���ɹ�������������
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
    for(t=1970;t<Calendar_Info->year;t++)    //��������ݵ��������
    {
			(Is_LeapYear(t))?(secCount+=31622400):(secCount+=31536000);////�����������
    }
    Calendar_Info->month-=1;//�ȼ���һ����������������������5��10�գ���ֻ��Ҫ��ǰ4���µ��������ټ���10�죬Ȼ�����������
    for(t=0;t<Calendar_Info->month;t++)
    {
			secCount+=(unsigned int)mon_table[t]*86400;//�·����������
			(Is_LeapYear(Calendar_Info->year)&&t==1)?(secCount+=86400):(0);//���꣬2�·�����һ���������
    }
    secCount+=(unsigned int)(Calendar_Info->day-1)*86400;//��ǰ�����ڵ���������ӣ���һ�컹û���꣬����-1��
    secCount+=(unsigned int)Calendar_Info->hour*3600;//Сʱ������
    secCount+=(unsigned int)Calendar_Info->min*60;//����������
    secCount+=Calendar_Info->sec;
    RTC_SetCounter(secCount);//����RTC��������ֵ
    RTC_WaitForLastTask();    //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_Get(&Calendar);//����ʱ��	
		return 0;
}

/*
�õ���ǰ��ʱ��
�ɹ�����0�����󷵻�����
*/
/////////////////////////////////////////////////////////////////////////////////////////////
void RTC_Get(Calendar_obj *Calendar_Info) 
{
		static unsigned short dayCount=0;
		unsigned int secCount=0;
		unsigned int tmp=0;
		unsigned short tmp1=0;
		secCount=RTC_GetCounter();
		tmp=secCount/86400;//�õ�����
		if(dayCount!=tmp)//����һ��
		{
				dayCount=tmp;
				tmp1=1970;//��1970�꿪ʼ
				while(tmp>=365)
				{
						if(Is_LeapYear(tmp1))//������
						{
							if(tmp>=366)
							{
								tmp-=366;
							}
							else
							{
								break; //�������������
							}								
						}
						else
						{
							 tmp-=365;//ƽ��
						}
						tmp1++;
				}
				Calendar_Info->year=tmp1;//�õ����
				tmp1=0;
				while(tmp>=28)//����һ����
				{
						if(Is_LeapYear(Calendar_Info->year)&&tmp1==1)//��������������ѭ��2��
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
								if(tmp>=mon_table[tmp1])//ƽ��
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
				Calendar_Info->month=tmp1+1;//�õ��·ݣ�tmp1=0��ʾ1�£�����Ҫ��1
				Calendar_Info->day=tmp+1;    //�õ����ڣ���Ϊ��һ�컹û���꣬����tmpֻ����ǰһ�죬������ʾ��ʱ��Ҫ��ʾ��������
		}
		tmp=secCount%86400;//�õ�������
		Calendar_Info->hour=tmp/3600;//Сʱ
		Calendar_Info->min=(tmp%3600)/60;//����
		Calendar_Info->sec=(tmp%3600)%60;//��
}
/*
RTCʱ���ж�
ÿ�봥��һ��
*/
//void RTC_IRQHandler(void)
//{                                
//    if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//�����ж�
//    {
//        RTC_ClearITPendingBit(RTC_IT_ALR);//�������ж�        
//    }                                                    
//    RTC_ClearITPendingBit(RTC_IT_OW);//�������ж�
//    RTC_WaitForLastTask();                                                   
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
void RTC_Thread(void)
{
	RTC_Get(&Calendar);//ʱ���ȡ
//	Calendar_UI(&Calendar);	      //��������ʱ��UI
}








