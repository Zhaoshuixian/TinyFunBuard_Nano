
#include "led.h"

/*
-------------设备对象id--------------------bool驱动量----------------------
身份注册-------------------->调用驱动函数--------------------->调用底层硬件接口
*/

led_st led[2];

////////////////////////////////////////////////////////////////////////
void hw_port_creat(unsigned char io_bool)           //硬件底层IO函数创建
{
//	io_bool?(RLED_SW(1)):(RLED_SW(0));
	io_bool?(BLED_SW(1)):(BLED_SW(0));
}

////////////////////////////////////////////////////////////////////////
void hw_creat_cb(void(*hw_port_cb)(unsigned char),unsigned char io_bool)//硬件底层IO函数回调
{
	hw_port_cb(io_bool);
}

///////////////////////////////////////////////////////////////////////
void hw_layer_cb(led_st *led_info) //
{
	hw_creat_cb(hw_port_creat,led_info->rt_intvl);
}

////////////////////////////////////////////////////////////////////////
void led_drive_implement(led_st *led_info,unsigned int TASK_SLICE)  //驱动效果实现
{
	if(0!=led_info->wink_num) //闪烁次数
	{
		led_info->rt_intvl=(led_info->rt_intvl+1)%(led_info->rtime/TASK_SLICE);//闪烁频率
	}
	else
	{
		led_info->rt_intvl=0; //
	}
	hw_layer_cb(led_info);  //底层硬件接口调用
}

void led_creat(led_st *led_info,unsigned char num,unsigned int run_time,unsigned int TASK_SLICE)
{
	led_info->wink_num=num;
	led_info->rtime=run_time;
	led_drive_implement(led_info,TASK_SLICE);
}

////////////////////////////////////////////////////////////////////////
void LED_Thread(void)
{
  led_creat(&led[0],1,500,LED_TASK_TIME); 
}




