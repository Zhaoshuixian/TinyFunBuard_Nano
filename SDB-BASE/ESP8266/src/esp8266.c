

#include "esp8266.h"
#include "bsp.h"
#include "stdio.h"

wifi_st  wifi;

//unsigned char ESP8266_SendCmd(const char *cmd,unsigned char cmd_len,const char *ack,unsigned short int ack_timeout)//��������
//{
//	unsigned char ret=0;  //״̬��־
// 
//	printf("%s\r\n",cmd);	//��������	
//	if(ack&&--ack_timeout)
//	{
////		if()
////		{
////			
////		}
//	}
//	printf("ack:%s\r\n",ack);
//}
//unsigned char ESP8266_SendData(unsigned char data,unsigned char data_len,const char *ack,unsigned short int ack_timeout)//��������
//{
//  unsigned char ret=0; //״̬��־

//	printf("AT+CIPSEND=%d,%d\r\n",data,data_len);	//��������
//	printf("ack:%s\r\n",ack);

//	if(ack_timeout==0)  ret=1; 
//	return ret;
//}


//char *ESP8266_AckCheck(const char *ack)   //Ӧ�����ݼ��
//{
//	
//}



