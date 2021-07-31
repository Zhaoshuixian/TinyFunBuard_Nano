

#ifndef  _LED_H__
#define  _LED_H__ 
 
#include "bsp.h"
#include "rrs.h"

typedef struct
{
	char *name;             //����
	unsigned char id;       //���ID
	unsigned char wink_num; //��˸����
	unsigned int  rtime;    //����ʱ��
	unsigned int  rt_intvl; //���м������˸Ƶ�ʣ�
	unsigned char bool_val; //Ӳ���㴫����������
	void (*hw_layer)(unsigned char); //Ӳ����ӿں���
}led_st;

extern led_st led[2];


void hw_port_creat(unsigned char io_bool);           //Ӳ���ײ�IO��������
void hw_creat_cb(void(*hw_port_cb)(unsigned char),unsigned char io_bool);//Ӳ���ײ�IO�����ص�
void hw_layer_cb(led_st *led_info); //
void led_drive_implement(led_st *led_info,unsigned int TASK_SLICE);//����Ч��ʵ��
void LED_Thread(void);

#endif


