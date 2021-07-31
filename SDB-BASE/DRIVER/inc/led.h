

#ifndef  _LED_H__
#define  _LED_H__ 
 
#include "bsp.h"
#include "rrs.h"

typedef struct
{
	char *name;             //名称
	unsigned char id;       //身份ID
	unsigned char wink_num; //闪烁次数
	unsigned int  rtime;    //运行时间
	unsigned int  rt_intvl; //运行间隔（闪烁频率）
	unsigned char bool_val; //硬件层传递驱动参数
	void (*hw_layer)(unsigned char); //硬件层接口函数
}led_st;

extern led_st led[2];


void hw_port_creat(unsigned char io_bool);           //硬件底层IO函数创建
void hw_creat_cb(void(*hw_port_cb)(unsigned char),unsigned char io_bool);//硬件底层IO函数回调
void hw_layer_cb(led_st *led_info); //
void led_drive_implement(led_st *led_info,unsigned int TASK_SLICE);//驱动效果实现
void LED_Thread(void);

#endif


