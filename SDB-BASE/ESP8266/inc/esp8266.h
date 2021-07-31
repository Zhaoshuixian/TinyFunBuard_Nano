

#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "pindef.h"
#include "stm32f10x_conf.h"


typedef struct
{
	const	char* ssid;        //�˺�����
	const	char* passedword;  //�˺�����
	const	char* encryp_type; //���ܷ�ʽ
	unsigned char work_mode; //����ģʽ
	
}wifi_st;


#endif


