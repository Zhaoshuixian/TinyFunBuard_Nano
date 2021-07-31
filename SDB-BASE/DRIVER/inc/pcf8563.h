#ifndef __PCF8563_H__
#define __PCF8563_H__

#include "stm32f10x.h"

/***************�Ĵ���ֵ�궨��***************/

#define PCF8563_ADDR_RD	0xA3    //�������ַ
#define PCF8563_ADDR_WR	0xA2    //д�����ַ


#define			CTRL_STU1				   0X00						//����״̬�Ĵ���1
#define			CTRL_STU2				   0X01						//����״̬�Ĵ���2
#define			SEC_REG					   0X02						//��Ĵ���
#define			MIN_REG					   0X03						//�ּĴ���
#define			HOUR_REG				   0X04						//Сʱ�Ĵ���
#define			DAY_REG					   0X05						//�ռĴ���
#define			WEEK_REG				   0X06						//�ܼĴ���
#define			MON_REG					   0X07						//�¼Ĵ���		
#define			YEAR_REG				   0X08						//��Ĵ���
#define			ALARM_MIN_REG			 0X09						//���ӱ����Ĵ���	
#define			ALARM_HOUR_REG		 0X0A						//Сʱ�����Ĵ���
#define			ALARM_DAY_REG			 0X0B						//�ձ����Ĵ���
#define			ALARM_WEEK_REG		 0X0C						//�ܱ����Ĵ���
#define			CLKOUT_REG				 0X0D						//ʱ������Ĵ���
#define			TIMER_REG				   0X0E						//��ʱ�����ƼĴ���
#define			COUNTER_REG				 0X0F						//�������������ƼĴ���

//��ַ����
#define			READ_ADDR				  0XA3						//8563��������ַ
#define			WRITE_ADDR				0XA2						//8563д������ַ

//�����
#define			START_CLOCK				0x00						//����ʱ��
#define AF (control2&0x1f)


//ʱ�����ýṹ��

#pragma pack(1)
typedef struct 
{	
	unsigned short int w_year;
	unsigned char w_month;
	unsigned char w_date;	
	unsigned char hour;
	unsigned char min;
	unsigned char sec;			
	unsigned char w_week;		
}Calendar_obj;	
#pragma pack()

extern unsigned char const mon_table[12];//�·��������ݱ�
void Alarm_Action(void);
unsigned char BCDtHEX(unsigned char data); //��BCD��ת��Ϊʮ������
unsigned char PCF8563_stop(void);//PCF8563ʱ��ָֹͣ��
unsigned char PCF8563_start(void);//PCF8563ʱ������ָ��
unsigned char PCF8563_ClearINT(void);////�жϲ��������TF��־λ
unsigned char PCF8563_ReadOneByte(unsigned short int ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void PCF8563_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite);		//ָ����ַд��һ���ֽ�
unsigned char PCF8563_WriteLenByte(unsigned short int WriteAddr,unsigned char *ptChar,unsigned char Len);//ָ����ַ��ʼд��ָ�����ȵ�����
unsigned char PCF8563_ReadLenByte(unsigned short int ReadAddr,unsigned char *ptChar,unsigned char Len);					//ָ����ַ��ʼ��ȡָ����������	
unsigned char Init_Bcd1(unsigned char dat);   //��һλʮ��������ת��ΪBCD�룬������
void Into_Bcd(unsigned char *p,unsigned char n);  //��ʮ����������ת��ΪBCD��
void PCF8563_RTC_Init(void);
void RTC_AutoUpData(void);//RTC�Զ�����
void RTC_AutoCheck(void);         //����ʱ�� 
void RTC_Set_F1(Calendar_obj *Calendar_info);
void RTC_Set_F2(Calendar_obj *Calendar_info);
unsigned char Calc_Week(Calendar_obj *Calendar_info);


//unsigned char Calc_Week(unsigned short int yeartxt,unsigned char monthtxt,unsigned char daytxt);
//void RTC_Set_F1(unsigned short int syear,unsigned char smon,unsigned char sday,unsigned char shour,unsigned char smin,unsigned char ssec);
//void RTC_Set_F2(unsigned short int syear,unsigned char smon,unsigned char sday,unsigned char sweek,unsigned char shour,unsigned char smin,unsigned char ssec);
void RTC_Get(void);
unsigned char BCDtHEX(unsigned char data);




#endif
