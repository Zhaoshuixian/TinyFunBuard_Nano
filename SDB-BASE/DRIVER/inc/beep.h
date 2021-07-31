

#ifndef __BEEP_H__
#define __BEEP_H__


typedef struct
{
	unsigned short int conter; //
	unsigned char num:7; //
	unsigned char en:1;//
	unsigned int rt;//
}buzzer_st;

extern buzzer_st buzzer;
void buzzer_driver(buzzer_st *me);//

#endif


