#ifndef __PCF8563_H__
#define __PCF8563_H__

#include "stm32f10x.h"

/***************寄存器值宏定义***************/

#define PCF8563_ADDR_RD	0xA3    //读命令地址
#define PCF8563_ADDR_WR	0xA2    //写命令地址


#define			CTRL_STU1				   0X00						//控制状态寄存器1
#define			CTRL_STU2				   0X01						//控制状态寄存器2
#define			SEC_REG					   0X02						//秒寄存器
#define			MIN_REG					   0X03						//分寄存器
#define			HOUR_REG				   0X04						//小时寄存器
#define			DAY_REG					   0X05						//日寄存器
#define			WEEK_REG				   0X06						//周寄存器
#define			MON_REG					   0X07						//月寄存器		
#define			YEAR_REG				   0X08						//年寄存器
#define			ALARM_MIN_REG			 0X09						//分钟报警寄存器	
#define			ALARM_HOUR_REG		 0X0A						//小时报警寄存器
#define			ALARM_DAY_REG			 0X0B						//日报警寄存器
#define			ALARM_WEEK_REG		 0X0C						//周报警寄存器
#define			CLKOUT_REG				 0X0D						//时钟输出寄存器
#define			TIMER_REG				   0X0E						//定时器控制寄存器
#define			COUNTER_REG				 0X0F						//倒数计数器控制寄存器

//地址定义
#define			READ_ADDR				  0XA3						//8563读操作地址
#define			WRITE_ADDR				0XA2						//8563写操作地址

//命令定义
#define			START_CLOCK				0x00						//启动时钟
#define AF (control2&0x1f)


//时间设置结构体

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

extern unsigned char const mon_table[12];//月份日期数据表
void Alarm_Action(void);
unsigned char BCDtHEX(unsigned char data); //将BCD码转化为十六进制
unsigned char PCF8563_stop(void);//PCF8563时钟停止指令
unsigned char PCF8563_start(void);//PCF8563时钟启动指令
unsigned char PCF8563_ClearINT(void);////中断产生后清除TF标志位
unsigned char PCF8563_ReadOneByte(unsigned short int ReadAddr);							//指定地址读取一个字节
void PCF8563_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite);		//指定地址写入一个字节
unsigned char PCF8563_WriteLenByte(unsigned short int WriteAddr,unsigned char *ptChar,unsigned char Len);//指定地址开始写入指定长度的数据
unsigned char PCF8563_ReadLenByte(unsigned short int ReadAddr,unsigned char *ptChar,unsigned char Len);					//指定地址开始读取指定长度数据	
unsigned char Init_Bcd1(unsigned char dat);   //将一位十六进制数转化为BCD码，并返回
void Into_Bcd(unsigned char *p,unsigned char n);  //将十六进制数组转化为BCD码
void PCF8563_RTC_Init(void);
void RTC_AutoUpData(void);//RTC自动更新
void RTC_AutoCheck(void);         //更新时间 
void RTC_Set_F1(Calendar_obj *Calendar_info);
void RTC_Set_F2(Calendar_obj *Calendar_info);
unsigned char Calc_Week(Calendar_obj *Calendar_info);


//unsigned char Calc_Week(unsigned short int yeartxt,unsigned char monthtxt,unsigned char daytxt);
//void RTC_Set_F1(unsigned short int syear,unsigned char smon,unsigned char sday,unsigned char shour,unsigned char smin,unsigned char ssec);
//void RTC_Set_F2(unsigned short int syear,unsigned char smon,unsigned char sday,unsigned char sweek,unsigned char shour,unsigned char smin,unsigned char ssec);
void RTC_Get(void);
unsigned char BCDtHEX(unsigned char data);




#endif
