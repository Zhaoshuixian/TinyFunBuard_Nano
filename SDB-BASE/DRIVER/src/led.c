
#include "led.h"

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
void hw_layer_cb(led_st *const me) //
{
	hw_creat_cb(hw_port_creat,me->rt_intvl);
}

////////////////////////////////////////////////////////////////////////
void led_driver(led_st *const me,unsigned int TASK_SLICE)  //����Ч��ʵ��
{
	if(0!=me->wink_num) //��˸����
	{
		me->rt_intvl=(me->rt_intvl+1)%(me->rtime/TASK_SLICE);//��˸Ƶ��
	}
	else
	{
		me->rt_intvl=0; //
	}
	hw_layer_cb(me);  //�ײ�Ӳ���ӿڵ���
}

void led_create(led_st *const me,unsigned char num,unsigned int run_time,unsigned int TASK_SLICE)
{
	me->wink_num=num;
	me->rtime=run_time;
	led_drive_implement(me,TASK_SLICE);
}

////////////////////////////////////////////////////////////////////////
void LED_Thread(void)
{
  led_create(&led[0],1,500,LED_TASK_TIME); 
}




