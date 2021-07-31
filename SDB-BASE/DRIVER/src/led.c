
#include "led.h"

/*
-------------�豸����id--------------------bool������----------------------
���ע��-------------------->������������--------------------->���õײ�Ӳ���ӿ�
*/

led_st led[2];

////////////////////////////////////////////////////////////////////////
void hw_port_creat(unsigned char io_bool)           //Ӳ���ײ�IO��������
{
//	io_bool?(RLED_SW(1)):(RLED_SW(0));
	io_bool?(BLED_SW(1)):(BLED_SW(0));
}

////////////////////////////////////////////////////////////////////////
void hw_creat_cb(void(*hw_port_cb)(unsigned char),unsigned char io_bool)//Ӳ���ײ�IO�����ص�
{
	hw_port_cb(io_bool);
}

///////////////////////////////////////////////////////////////////////
void hw_layer_cb(led_st *led_info) //
{
	hw_creat_cb(hw_port_creat,led_info->rt_intvl);
}

////////////////////////////////////////////////////////////////////////
void led_drive_implement(led_st *led_info,unsigned int TASK_SLICE)  //����Ч��ʵ��
{
	if(0!=led_info->wink_num) //��˸����
	{
		led_info->rt_intvl=(led_info->rt_intvl+1)%(led_info->rtime/TASK_SLICE);//��˸Ƶ��
	}
	else
	{
		led_info->rt_intvl=0; //
	}
	hw_layer_cb(led_info);  //�ײ�Ӳ���ӿڵ���
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




