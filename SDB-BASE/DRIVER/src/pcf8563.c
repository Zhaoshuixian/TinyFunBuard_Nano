

#include "pcf8563.h"
#include "i2c.h"
#include "delay.h"
#include "ui.h"

Calendar_obj Calendar={2018,10,15,12,12,0,0};

////////////////////////////////////////////////////////////////////////////////////////////////////
//PCF8563ʱ��ָֹͣ�� 
unsigned char pcf8563_stop(void)
{
 	unsigned char stopcode=0x20;
 	if(0==PCF8563_WriteLenByte(CTRL_STU1,&stopcode,1))
		return 0;
 	else return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//PCF8563ʱ������ָ��
unsigned char pcf8563_start(void)
{
 	unsigned char startcode=0x00;
	if(0==PCF8563_WriteLenByte(CTRL_STU1,&startcode,1)) 
		return 0;
 	else return 1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////�жϲ��������TF��־λ
unsigned char pcf8563_clear_interrupt(void)
{
 	unsigned char temp=0x12;
	if(0==PCF8563_WriteLenByte(CTRL_STU2,&temp,1)) return 0;
 	else return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//��PCF8563ָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
unsigned char PCF8563_ReadOneByte(unsigned short int ReadAddr)
{				  
	unsigned char temp=0;	

    IIC2_Start();  
	
	IIC2_Send_Byte(PCF8563_ADDR_WR);   //����������ַPCF8563_Read_Add,д���� 	
	IIC2_Wait_Ack();	
    
	IIC2_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC2_Wait_Ack();
	
	IIC2_Start();  	 	   
	IIC2_Send_Byte(PCF8563_ADDR_RD);           //�������ģʽ		   
	IIC2_Wait_Ack();	 
	temp=IIC2_Read_Byte(0);		
	IIC2_NAck();//���ͷ�Ӧλ	
	IIC2_Stop();//����һ��ֹͣ����	    
	return temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void PCF8563_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
  IIC2_Start();  
	IIC2_Send_Byte(PCF8563_ADDR_WR);   //����������ַPCF8563_Write_Add,д���� 
	IIC2_Wait_Ack();	   
  IIC2_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC2_Wait_Ack();  		    	   
  IIC2_Stop();//����һ��ֹͣ���� 
  xDelay_ms(10);	 
}

////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char PCF8563_WriteLenByte(unsigned short int WriteAddr,unsigned char *ptChar,unsigned char Len)
{  	
	unsigned char t;
	 
	for(t=0;t<Len;t++)
	{
		PCF8563_WriteOneByte(WriteAddr,*ptChar);
		WriteAddr++;
		ptChar++;	//
	}	
	 
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PCF8563_ReadLenByte(unsigned short int ReadAddr,unsigned char *ptChar,unsigned char Len)
{  	

	unsigned char t,*temp;
	temp = ptChar;          
	for(t=0;t<Len;t++)
	{
		*ptChar++=PCF8563_ReadOneByte(ReadAddr++);
	}
	
	//���ʱ�䷵���Ƿ���ȷ
	//������7ʱ��ΪֵΪ0
	if((temp[5]==0)||(temp[3]==0))
	{
		return 1;
	}
	return 0;
}

/////////////////////////ʮ����תBCD//////////////////////////////////////

unsigned char Dec_ToBcd(unsigned char Dec_Val)
{
	unsigned char Bcd_Val;
	Bcd_Val=(((Dec_Val/10)<<4)|(Dec_Val%10));
	return Bcd_Val;
}

///////////////////////////BCDתʮ����////////////////////////////////////

unsigned char Bcd_ToDec(unsigned char Bcd_Val)
{
	unsigned char Dec_Val;
	Dec_Val=((((Bcd_Val&0xf0)>>4)*10)+(Bcd_Val&0x0f));
	return Dec_Val;
}

////////////////////��ʼ��////////////////////////////
void PCF8563_RTC_Init(void)
{
	
	PCF8563_IIC_Init();
	PCF8563_start();	//ʱ������
	//��һ������
	if (PCF8563_ReadOneByte(ALARM_MIN_REG) != 0x88)		//оƬ�Ƿ��Ѿ����ù���ʹ�÷��ӱ����Ĵ�����Ϊ�жϣ��жϹر�
	{	 			
 		PCF8563_WriteOneByte(ALARM_MIN_REG,0X88);//������д��PCF8563��Ӧ�Ĵ���
		RTC_Set_F1(&Calendar);   //����ʱ�䣬�ꡢ�¡��ա�ʱ���֡��롣
	}  
//	RTC_Get();//����ʱ��
}


//////////////////////////////////////////RTCʱ������д��/////////////////////////////////////////////////////
void RTC_Set_F1(Calendar_obj *Calendar_info)
{
	 unsigned char b_time[7]={0};
	//8563ʹ�õ���BCD�룬������Ҫ��ʮ������ת����BCD��
	 (19==(Calendar_info->w_year/100))?(Calendar_info->w_month|=0x80):(Calendar_info->w_month&=~0x80);//�ж�����
	 b_time[6]=Dec_ToBcd((unsigned char)(Calendar_info->w_year%100));//0~99
	 b_time[5]=Dec_ToBcd(Calendar_info->w_month);//���ͣ�bit7��+�·�  bit7=0��ʾΪ20xx,bit7=1��ʾ19xx;
	 b_time[3]=Dec_ToBcd(Calendar_info->w_date);
	 if(7==Calc_Week(Calendar_info))//����)////����ķ����ɭ�㹫ʽ1~7��ʾ����һ��������//PCF8563������˳�� 0~6-->������~������
	 {
		 b_time[4]=0;
	 }
	 else
	 {
		 b_time[4]=Calc_Week(Calendar_info);
	 }	
	 b_time[4]=Dec_ToBcd(b_time[4]);
	 b_time[2]=Dec_ToBcd(Calendar_info->hour);
	 b_time[1]=Dec_ToBcd(Calendar_info->min);
	 b_time[0]=Dec_ToBcd(Calendar_info->sec);
	 //��д���ݷ�ʽ
	 //1.ÿ�ζ�д�����Ƕ�ֵ�ַ�Ĵ��������Զ�����ƫ��
	 //2.�����ַ��д
	 PCF8563_WriteOneByte(SEC_REG	,b_time[0]);
	 PCF8563_WriteOneByte(MIN_REG	,b_time[1]);
	 PCF8563_WriteOneByte(HOUR_REG,b_time[2]);
	 PCF8563_WriteOneByte(DAY_REG, b_time[3]);
	 PCF8563_WriteOneByte(WEEK_REG,b_time[4]);
	 PCF8563_WriteOneByte(MON_REG, b_time[5]);
	 PCF8563_WriteOneByte(YEAR_REG,b_time[6]);

}

//////////////////////////////////////////RTCʱ������д�루�������βΣ�/////////////////////////////////////////////////////
void RTC_Set_F2(Calendar_obj *Calendar_info)
{
	//��ӦPCF8563������˳�� 0~6-->������~������
	 unsigned char b_time[7]={0};
	//8563ʹ�õ���BCD�룬������Ҫ��ʮ������ת����BCD��
	 (19==(Calendar_info->w_year/100))?(Calendar_info->w_month|=0x80):(Calendar_info->w_month&=~0x80);//�ж�����
	 b_time[6]=Dec_ToBcd((unsigned char)(Calendar_info->w_year%100));//0~99
	 b_time[5]=Dec_ToBcd(Calendar_info->w_month);//���ͣ�bit7��+�·�  bit7=0��ʾΪ20xx,bit7=1��ʾ19xx;
	 b_time[3]=Dec_ToBcd(Calendar_info->w_date);
	 if(0==Calendar_info->w_week)//����)
	 {
		 b_time[4]=7;
	 }
	 else
	 {
		 b_time[4]=Calendar_info->w_week;
	 }	
	 b_time[4]=Dec_ToBcd(b_time[4]);
	 b_time[2]=Dec_ToBcd(Calendar_info->hour);
	 b_time[1]=Dec_ToBcd(Calendar_info->min);
	 b_time[0]=Dec_ToBcd(Calendar_info->sec);
	 //��д���ݷ�ʽ
	 //1.ÿ�ζ�д�����Ƕ�ֵ�ַ�Ĵ��������Զ�����ƫ��
	 //2.�����ַ��д
	 PCF8563_WriteOneByte(SEC_REG	,b_time[0]);
	 PCF8563_WriteOneByte(MIN_REG	,b_time[1]);
	 PCF8563_WriteOneByte(HOUR_REG,b_time[2]);
	 PCF8563_WriteOneByte(DAY_REG, b_time[3]);
	 PCF8563_WriteOneByte(WEEK_REG,b_time[4]);
	 PCF8563_WriteOneByte(MON_REG, b_time[5]);
	 PCF8563_WriteOneByte(YEAR_REG,b_time[6]);

}

/////////////////////////��RTC////////////////////////////////////////////////////
void RTC_Get(void)
{
    unsigned char temp[7]={0};
    temp[0]= PCF8563_ReadOneByte(SEC_REG)&0x7F;
		temp[1]= PCF8563_ReadOneByte(MIN_REG)&0x7F;
		temp[2]= PCF8563_ReadOneByte(HOUR_REG)&0x3F;
		temp[3]= PCF8563_ReadOneByte(DAY_REG)&0x3F;
		temp[4]= PCF8563_ReadOneByte(WEEK_REG)&0x07;//����
		temp[5]= PCF8563_ReadOneByte(MON_REG)&0x1F;
		temp[6]= PCF8563_ReadOneByte(YEAR_REG);
		
	  Calendar.sec=Bcd_ToDec(temp[0]);
		Calendar.min=Bcd_ToDec(temp[1]);	
		Calendar.hour=Bcd_ToDec(temp[2]);
		Calendar.w_date=Bcd_ToDec(temp[3]);
		Calendar.w_week=Bcd_ToDec(temp[4]);
		Calendar.w_month=Bcd_ToDec(temp[5]);		
		Calendar.w_year=Bcd_ToDec(temp[6]);
		//����ķ����ɭ�㹫ʽ1~7��ʾ����һ��������//PCF8563������˳�� 0~6-->������~������
		if(7==Calendar.w_week)   Calendar.w_week=0;	//����Э��˳�����
		(0x80==(Calendar.w_month&0x80))?(Calendar.w_year+=1900):(Calendar.w_year+=2000);
		Calendar_UI(&Calendar);	//��������ʱ��UI
}
/////////////////////////////////////��������///////////////////////////////////////////////////
unsigned char Calc_Week(Calendar_obj *Calendar_info)
{
	unsigned int weektxt;//���ڱ���
	unsigned short int virt_yeartxt;//����һ�����������ڻ�ķ����ɭ��ʽ�������
	unsigned char virt_monthtxt;//����һ�����������ڻ�ķ����ɭ��ʽ���±���
	virt_yeartxt=Calendar_info->w_year;//����� ������
	virt_monthtxt=Calendar_info->w_month;//����� ������
	//------------------------------ͨ�������������������------------------------------------------------------
	//��һ�ºͶ��¿�������һ���ʮ���º�ʮ���£����������2004-1-10����ɣ�2003-13-10�����빫ʽ���㡣
	if((1==Calendar_info->w_month)||(2==Calendar_info->w_month))
	{		
		virt_monthtxt+=12; //���� 13/14
		virt_yeartxt-=1;//����� ������
	}
	//��ķ����ɭ��ʽ �˹�ʽ�� 0~6��ʾ����һ��������
	weektxt= (Calendar_info->w_date + 2*virt_monthtxt + 3*(virt_monthtxt+1)/5 + virt_yeartxt+virt_yeartxt/4 - virt_yeartxt/100 + virt_yeartxt/400)%7;
	weektxt+=1;//1~7��ʾ����һ��������
	return (unsigned char)weektxt;
}



