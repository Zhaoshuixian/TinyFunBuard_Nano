
#include "at24cxx.h"
#include "lcd.h"
#include "delay.h"
#include "bsp.h"
#include "i2c.h"


unsigned char EEPROM_BUF[256];/*EEPROM�洢�ռ� 256Byte x8bit*/

void DefaultConfig(void)
{
//	OLED_Init();
	EEPROM_BUF[255]=0X55;
	AT24CXX_Write(255,(unsigned char*)&EEPROM_BUF[255],1);//��*�ֽڴ���ʼ,д��SIZE���ȵ����� 	
}

////////////////////////////////////////////////////////////////////
void AT24Cxx_AutoStore(void)
{
//  //////////////////����ʱ��///////////////////////////////	
//	if(MWC.FtrNet.tVal!=NeedMem_Type.FtrNet_Run_Time)
//	{
//		memcpy(&NeedMem_Type.FtrNet_Run_Time,&MWC.FtrNet.tVal,sizeof(NeedMem_Type.FtrNet_Run_Time));
//		EEPROM_BUF[0]=(NeedMem_Type.FtrNet_Run_Time&0xFF);
//		EEPROM_BUF[1]=(NeedMem_Type.FtrNet_Run_Time>>8);
//		AT24CXX_Write(0,(unsigned char*)&EEPROM_BUF[0],2);//��*�ֽڴ���ʼ,д��SIZE���ȵ����� 
//	}
////////////////////////��ʱʱ��//////////////////////////////////	
//	if(ALARM_SET_MODE==MWC.System.Set.onMode)
//	{
//		if(MWC.System.Alarm.Close_Clock.Hour!=NeedMem_Type.Alarm_Close_HourVal)
//		{
//			memcpy(&NeedMem_Type.Alarm_Close_HourVal,&MWC.System.Alarm.Close_Clock.Hour,sizeof(NeedMem_Type.Alarm_Close_HourVal));
//			EEPROM_BUF[2]=NeedMem_Type.Alarm_Close_HourVal;
//			AT24CXX_Write(2,(unsigned char*)&EEPROM_BUF[2],1);//��*�ֽڴ���ʼ,д��SIZE���ȵ����� 
//		}
//		if(MWC.System.Alarm.Close_Clock.Min!=NeedMem_Type.Alarm_Close_MinVal)
//		{
//			memcpy(&NeedMem_Type.Alarm_Close_MinVal,&MWC.System.Alarm.Close_Clock.Min,sizeof(NeedMem_Type.Alarm_Close_MinVal));
//			EEPROM_BUF[3]=NeedMem_Type.Alarm_Close_MinVal;
//			AT24CXX_Write(3,(unsigned char*)&EEPROM_BUF[3],1);//��*�ֽڴ���ʼ,д��SIZE���ȵ����� 
//		}
//		if(MWC.System.Alarm.Open_Clock.Hour!=NeedMem_Type.Alarm_Open_HourVal)
//		{
//			memcpy(&NeedMem_Type.Alarm_Open_HourVal,&MWC.System.Alarm.Open_Clock.Hour,sizeof(NeedMem_Type.Alarm_Open_HourVal));
//			EEPROM_BUF[4]=NeedMem_Type.Alarm_Open_HourVal;
//			AT24CXX_Write(4,(unsigned char*)&EEPROM_BUF[4],1);//��*�ֽڴ���ʼ,д��SIZE���ȵ����� 
//		}
//		if(MWC.System.Alarm.Open_Clock.Min!=NeedMem_Type.Alarm_Open_MinVal)
//		{
//			memcpy(&NeedMem_Type.Alarm_Open_MinVal,&MWC.System.Alarm.Open_Clock.Min,sizeof(NeedMem_Type.Alarm_Open_MinVal));
//			EEPROM_BUF[5]=NeedMem_Type.Alarm_Open_MinVal;
//			AT24CXX_Write(5,(unsigned char*)&EEPROM_BUF[5],1);//��*�ֽڴ���ʼ,д��SIZE���ȵ����� 
//		}		
//  }
//////////////////////////�̵���״̬//////////////////////////////////////
//	if(MWC.Key.Obj.Pwr_SW)//�ػ�״̬�±����״̬
//	{
//		if(MWC.Fan.Sta!=NeedMem_Type.Fan_Run_Sta)
//		{
//			memcpy(&NeedMem_Type.Fan_Run_Sta,&MWC.Fan.Sta,sizeof(NeedMem_Type.Fan_Run_Sta));
//			EEPROM_BUF[6]=NeedMem_Type.Fan_Run_Sta;	
//			AT24CXX_Write(6,(unsigned char*)&EEPROM_BUF[6],1);//��*�ֽڴ���ʼ,д��SIZE���ȵ����� 		
//		}
//		if(MWC.Fan.Lev!=NeedMem_Type.Fan_Run_Lev) 
//		{
//			memcpy(&NeedMem_Type.Fan_Run_Lev,&MWC.Fan.Lev,sizeof(NeedMem_Type.Fan_Run_Lev));
//			EEPROM_BUF[7]=NeedMem_Type.Fan_Run_Lev;
//			AT24CXX_Write(7,(unsigned char*)&EEPROM_BUF[7],1);//��*�ֽڴ���ʼ,д��SIZE���ȵ����� 
//		}
//  }
/////////////////////////485ͨ�Ų���///////////////////////////////
//	if(MWC.RS485.Dev_Addr!=NeedMem_Type.Dev_Com_Id)
//	{
//		memcpy(&NeedMem_Type.Dev_Com_Id,&MWC.RS485.Dev_Addr,sizeof(NeedMem_Type.Dev_Com_Id));
//		EEPROM_BUF[8]=NeedMem_Type.Dev_Com_Id;
//		AT24CXX_Write(8,(unsigned char*)&EEPROM_BUF[8],1);//��*�ֽڴ���ʼ,д��SIZE���ȵ����� 
//	}
//	if(MWC.RS485.Baud!=NeedMem_Type.Dev_Com_Baud)
//	{
//		memcpy(&NeedMem_Type.Dev_Com_Baud,&MWC.RS485.Baud,sizeof(NeedMem_Type.Dev_Com_Baud));
//		EEPROM_BUF[9]=NeedMem_Type.Dev_Com_Baud&0xFF;
//		EEPROM_BUF[10]=(NeedMem_Type.Dev_Com_Baud>>8)&0xFF;
//		EEPROM_BUF[11]=(NeedMem_Type.Dev_Com_Baud>>16)&0xFF;
//		EEPROM_BUF[12]=(NeedMem_Type.Dev_Com_Baud>>24)&0xFF;
//		AT24CXX_Write(9,(unsigned char*)&EEPROM_BUF[9],4);//��*�ֽڴ���ʼ,д��SIZE���ȵ����� 
//	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
unsigned char AT24CXX_ReadOneByte(unsigned short int ReadAddr)
{				  
	unsigned char temp=0;		  	    																 
    IIC1_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC1_Send_Byte(0XA0);	   //����д����
		IIC1_Wait_Ack();
		IIC1_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
		IIC1_Wait_Ack();		 
	}else 
	{
		IIC1_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	
	}		

	IIC1_Wait_Ack(); 
  IIC1_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC1_Wait_Ack();	    
	IIC1_Start();  	 	   
	IIC1_Send_Byte(0XA1);           //�������ģʽ			   
	IIC1_Wait_Ack();	 
    temp=IIC1_Read_Byte(0);		   
    IIC1_Stop();//����һ��ֹͣ����	    
	return temp;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
    IIC1_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC1_Send_Byte(0XA0);	    //����д����
		IIC1_Wait_Ack();
		IIC1_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
		IIC1_Wait_Ack();		 
	}else
	{
		IIC1_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 
	}	 
	IIC1_Wait_Ack();	   
  IIC1_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC1_Wait_Ack(); 	 										  		   
	IIC1_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC1_Wait_Ack();  		    	   
  IIC1_Stop();//����һ��ֹͣ���� 
	xDelay_ms(10);	 
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(unsigned short int WriteAddr,unsigned int DataToWrite,unsigned char Len)
{  	
	unsigned char t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
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
//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(unsigned short int ReadAddr,unsigned char *pBuffer,unsigned short int NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
} 


//////////////////////////////////////////////////////////////////////////////////////////////////
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
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
	 AT24_IIC_Init();                //��ʼ��IIC��IO��	
	if(AT24CXX_Check())//û�д洢��־
	{
		 DefaultConfig();//����ʾ�״�ʹ�ã�������ȱʡֵ
	}
	else
	{
		AT24CXX_Read(0,EEPROM_BUF,256);
	}
}
