

#ifndef __BEEP_H__
#define __BEEP_H__


typedef struct
{
	unsigned short int conter; //蜂鸣间隔累加器
	unsigned char num:7; //蜂鸣次数
	unsigned char en:1;//运行状态
	unsigned int rt;//运行时间
}Beep_st;

extern Beep_st Beep;
void Beep_Thread(Beep_st *Beep_info);//蜂鸣驱动

#endif


