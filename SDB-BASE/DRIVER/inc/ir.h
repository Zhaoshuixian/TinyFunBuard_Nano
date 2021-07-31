#ifndef __IR_H__
#define __IR_H__


#include "pindef.h"
#include "stm32f10x_conf.h"

/*****ң�ؼ�ֵ�����****/
#define  PWR_SW   0XA2
#define  MENU     0XE2
#define  TEST     0X22
#define  ADD      0X02
#define  REDUCE   0X98
#define  RECALL   0XC2
#define  PREVIOUS 0XE0
#define  PLAY     0XA8
#define  NEXT     0X90
#define  CANCEL   0XB0
#define  ZERO     0X68
#define  ONE      0X30
#define  TWO      0X18
#define  THREE    0X7A
#define  FOUR     0X10
#define  FIVE     0X38
#define  SIX      0X5A
#define  SEVEN    0X42
#define  EIGHT    0X4A
#define  NINE     0X52

/***************************/

typedef struct
{
	
	unsigned char cmd;
	unsigned char cmdn;
	unsigned char addr;
	unsigned char addrn;
}data_st;

typedef struct
{
	unsigned char rec_num;
	unsigned char valid_data;
	unsigned char rec_status;
	unsigned char ui_cflag;
	unsigned int  rec_data;
	unsigned int  rec_dval;
	unsigned int  ui_rtime;
  
	data_st data;
}ir_st;

#define IR_IRQHandler  TIM1_CC_IRQHandler  /*�˴�����ʹ��TIM1_UP_IRQHandler*/

extern infrade_st iinfrader;


#endif 

