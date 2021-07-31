
#include "at24cxx.h"
#include "lcd.h"
#include "delay.h"
#include "bsp.h"
#include "i2c.h"


unsigned char EEPROM_BUF[256];/*EEPROM存储空间 256Byte x8bit*/

void DefaultConfig(void)
{
//	OLED_Init();
	EEPROM_BUF[255]=0X55;
	AT24CXX_Write(255,(unsigned char*)&EEPROM_BUF[255],1);//从*字节处开始,写入SIZE长度的数据 	
}

////////////////////////////////////////////////////////////////////
void AT24Cxx_AutoStore(void)
{
//  //////////////////滤网时间///////////////////////////////	
//	if(MWC.FtrNet.tVal!=NeedMem_Type.FtrNet_Run_Time)
//	{
//		memcpy(&NeedMem_Type.FtrNet_Run_Time,&MWC.FtrNet.tVal,sizeof(NeedMem_Type.FtrNet_Run_Time));
//		EEPROM_BUF[0]=(NeedMem_Type.FtrNet_Run_Time&0xFF);
//		EEPROM_BUF[1]=(NeedMem_Type.FtrNet_Run_Time>>8);
//		AT24CXX_Write(0,(unsigned char*)&EEPROM_BUF[0],2);//从*字节处开始,写入SIZE长度的数据 
//	}
////////////////////////定时时间//////////////////////////////////	
//	if(ALARM_SET_MODE==MWC.System.Set.onMode)
//	{
//		if(MWC.System.Alarm.Close_Clock.Hour!=NeedMem_Type.Alarm_Close_HourVal)
//		{
//			memcpy(&NeedMem_Type.Alarm_Close_HourVal,&MWC.System.Alarm.Close_Clock.Hour,sizeof(NeedMem_Type.Alarm_Close_HourVal));
//			EEPROM_BUF[2]=NeedMem_Type.Alarm_Close_HourVal;
//			AT24CXX_Write(2,(unsigned char*)&EEPROM_BUF[2],1);//从*字节处开始,写入SIZE长度的数据 
//		}
//		if(MWC.System.Alarm.Close_Clock.Min!=NeedMem_Type.Alarm_Close_MinVal)
//		{
//			memcpy(&NeedMem_Type.Alarm_Close_MinVal,&MWC.System.Alarm.Close_Clock.Min,sizeof(NeedMem_Type.Alarm_Close_MinVal));
//			EEPROM_BUF[3]=NeedMem_Type.Alarm_Close_MinVal;
//			AT24CXX_Write(3,(unsigned char*)&EEPROM_BUF[3],1);//从*字节处开始,写入SIZE长度的数据 
//		}
//		if(MWC.System.Alarm.Open_Clock.Hour!=NeedMem_Type.Alarm_Open_HourVal)
//		{
//			memcpy(&NeedMem_Type.Alarm_Open_HourVal,&MWC.System.Alarm.Open_Clock.Hour,sizeof(NeedMem_Type.Alarm_Open_HourVal));
//			EEPROM_BUF[4]=NeedMem_Type.Alarm_Open_HourVal;
//			AT24CXX_Write(4,(unsigned char*)&EEPROM_BUF[4],1);//从*字节处开始,写入SIZE长度的数据 
//		}
//		if(MWC.System.Alarm.Open_Clock.Min!=NeedMem_Type.Alarm_Open_MinVal)
//		{
//			memcpy(&NeedMem_Type.Alarm_Open_MinVal,&MWC.System.Alarm.Open_Clock.Min,sizeof(NeedMem_Type.Alarm_Open_MinVal));
//			EEPROM_BUF[5]=NeedMem_Type.Alarm_Open_MinVal;
//			AT24CXX_Write(5,(unsigned char*)&EEPROM_BUF[5],1);//从*字节处开始,写入SIZE长度的数据 
//		}		
//  }
//////////////////////////继电器状态//////////////////////////////////////
//	if(MWC.Key.Obj.Pwr_SW)//关机状态下保存该状态
//	{
//		if(MWC.Fan.Sta!=NeedMem_Type.Fan_Run_Sta)
//		{
//			memcpy(&NeedMem_Type.Fan_Run_Sta,&MWC.Fan.Sta,sizeof(NeedMem_Type.Fan_Run_Sta));
//			EEPROM_BUF[6]=NeedMem_Type.Fan_Run_Sta;	
//			AT24CXX_Write(6,(unsigned char*)&EEPROM_BUF[6],1);//从*字节处开始,写入SIZE长度的数据 		
//		}
//		if(MWC.Fan.Lev!=NeedMem_Type.Fan_Run_Lev) 
//		{
//			memcpy(&NeedMem_Type.Fan_Run_Lev,&MWC.Fan.Lev,sizeof(NeedMem_Type.Fan_Run_Lev));
//			EEPROM_BUF[7]=NeedMem_Type.Fan_Run_Lev;
//			AT24CXX_Write(7,(unsigned char*)&EEPROM_BUF[7],1);//从*字节处开始,写入SIZE长度的数据 
//		}
//  }
/////////////////////////485通信参数///////////////////////////////
//	if(MWC.RS485.Dev_Addr!=NeedMem_Type.Dev_Com_Id)
//	{
//		memcpy(&NeedMem_Type.Dev_Com_Id,&MWC.RS485.Dev_Addr,sizeof(NeedMem_Type.Dev_Com_Id));
//		EEPROM_BUF[8]=NeedMem_Type.Dev_Com_Id;
//		AT24CXX_Write(8,(unsigned char*)&EEPROM_BUF[8],1);//从*字节处开始,写入SIZE长度的数据 
//	}
//	if(MWC.RS485.Baud!=NeedMem_Type.Dev_Com_Baud)
//	{
//		memcpy(&NeedMem_Type.Dev_Com_Baud,&MWC.RS485.Baud,sizeof(NeedMem_Type.Dev_Com_Baud));
//		EEPROM_BUF[9]=NeedMem_Type.Dev_Com_Baud&0xFF;
//		EEPROM_BUF[10]=(NeedMem_Type.Dev_Com_Baud>>8)&0xFF;
//		EEPROM_BUF[11]=(NeedMem_Type.Dev_Com_Baud>>16)&0xFF;
//		EEPROM_BUF[12]=(NeedMem_Type.Dev_Com_Baud>>24)&0xFF;
//		AT24CXX_Write(9,(unsigned char*)&EEPROM_BUF[9],4);//从*字节处开始,写入SIZE长度的数据 
//	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
unsigned char AT24CXX_ReadOneByte(unsigned short int ReadAddr)
{				  
	unsigned char temp=0;		  	    																 
    IIC1_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC1_Send_Byte(0XA0);	   //发送写命令
		IIC1_Wait_Ack();
		IIC1_Send_Byte(ReadAddr>>8);//发送高地址
		IIC1_Wait_Ack();		 
	}else 
	{
		IIC1_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	
	}		

	IIC1_Wait_Ack(); 
  IIC1_Send_Byte(ReadAddr%256);   //发送低地址
	IIC1_Wait_Ack();	    
	IIC1_Start();  	 	   
	IIC1_Send_Byte(0XA1);           //进入接收模式			   
	IIC1_Wait_Ack();	 
    temp=IIC1_Read_Byte(0);		   
    IIC1_Stop();//产生一个停止条件	    
	return temp;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
    IIC1_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC1_Send_Byte(0XA0);	    //发送写命令
		IIC1_Wait_Ack();
		IIC1_Send_Byte(WriteAddr>>8);//发送高地址
		IIC1_Wait_Ack();		 
	}else
	{
		IIC1_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
	}	 
	IIC1_Wait_Ack();	   
  IIC1_Send_Byte(WriteAddr%256);   //发送低地址
	IIC1_Wait_Ack(); 	 										  		   
	IIC1_Send_Byte(DataToWrite);     //发送字节							   
	IIC1_Wait_Ack();  		    	   
  IIC1_Stop();//产生一个停止条件 
	xDelay_ms(10);	 
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(unsigned short int WriteAddr,unsigned int DataToWrite,unsigned char Len)
{  	
	unsigned char t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
unsigned int AT24CXX_ReadLenByte(unsigned short int ReadAddr,unsigned char Len)
{  	
	unsigned char t;
	unsigned int temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
unsigned char AT24CXX_Check(void)
{
	unsigned char err=1;   
	if(0X55==AT24CXX_ReadOneByte(255))
	{
		 err=0;		
	}		
	else
	{
     err=1;	
	}
  return err;	
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(unsigned short int ReadAddr,unsigned char *pBuffer,unsigned short int NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
} 


//////////////////////////////////////////////////////////////////////////////////////////////////
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(unsigned short int WriteAddr,unsigned char *pBuffer,unsigned short int NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AT24CXX_Init(void)
{
	 AT24_IIC_Init();                //初始化IIC的IO口	
	if(AT24CXX_Check())//没有存储标志
	{
		 DefaultConfig();//即表示首次使用，则配置缺省值
	}
	else
	{
		AT24CXX_Read(0,EEPROM_BUF,256);
	}
}
