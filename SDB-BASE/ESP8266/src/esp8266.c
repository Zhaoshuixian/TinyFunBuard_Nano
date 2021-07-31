

#include "esp8266.h"
#include "bsp.h"
#include "stdio.h"

wifi_st  wifi;

//unsigned char ESP8266_SendCmd(const char *cmd,unsigned char cmd_len,const char *ack,unsigned short int ack_timeout)//发送命令
//{
//	unsigned char ret=0;  //状态标志
// 
//	printf("%s\r\n",cmd);	//发送命令	
//	if(ack&&--ack_timeout)
//	{
////		if()
////		{
////			
////		}
//	}
//	printf("ack:%s\r\n",ack);
//}
//unsigned char ESP8266_SendData(unsigned char data,unsigned char data_len,const char *ack,unsigned short int ack_timeout)//发送数据
//{
//  unsigned char ret=0; //状态标志

//	printf("AT+CIPSEND=%d,%d\r\n",data,data_len);	//发送数据
//	printf("ack:%s\r\n",ack);

//	if(ack_timeout==0)  ret=1; 
//	return ret;
//}


//char *ESP8266_AckCheck(const char *ack)   //应答内容检查
//{
//	
//}



