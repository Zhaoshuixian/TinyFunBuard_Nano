

#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "pindef.h"
#include "stm32f10x_conf.h"


typedef struct
{
	const	char* ssid;        //账号名称
	const	char* passedword;  //账号密码
	const	char* encryp_type; //加密方式
	unsigned char work_mode; //工作模式
	
}wifi_st;


#endif


