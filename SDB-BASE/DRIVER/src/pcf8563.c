

#include "pcf8563.h"
#include "i2c.h"
#include "delay.h"
#include "ui.h"

Calendar_obj Calendar={2018,10,15,12,12,0,0};

////////////////////////////////////////////////////////////////////////////////////////////////////
//PCF8563时钟停止指令 
unsigned char PCF8563_stop(void)
{
 	unsigned char stopcode=0x20;
 	if(0==PCF8563_WriteLenByte(CTRL_STU1,&stopcode,1))
		return 0;
 	else return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//PCF8563时钟启动指令
unsigned char PCF8563_start(void)
{
 	unsigned char startcode=0x00;
	if(0==PCF8563_WriteLenByte(CTRL_STU1,&startcode,1)) 
		return 0;
 	else return 1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////中断产生后清除TF标志位
unsigned char PCF8563_ClearINT(void)
{
 	unsigned char temp=0x12;
	if(0==PCF8563_WriteLenByte(CTRL_STU2,&temp,1))
		return 0;
 	else return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//在PCF8563指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
unsigned char PCF8563_ReadOneByte(unsigned short int ReadAddr)
{				  
	unsigned char temp=0;	
  IIC2_Start();  
	
	IIC2_Send_Byte(PCF8563_ADDR_WR);   //发送器件地址PCF8563_Read_Add,写数据 	
	IIC2_Wait_Ack();	
    
	IIC2_Send_Byte(ReadAddr%256);   //发送低地址
	IIC2_Wait_Ack();
	
	IIC2_Start();  	 	   
	IIC2_Send_Byte(PCF8563_ADDR_RD);           //进入接收模式		   
	IIC2_Wait_Ack();	 
	temp=IIC2_Read_Byte(0);		
	IIC2_NAck();//发送非应位	
	IIC2_Stop();//产生一个停止条件	    
	return temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//在PCF8563指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void PCF8563_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
  IIC2_Start();  
	IIC2_Send_Byte(PCF8563_ADDR_WR);   //发送器件地址PCF8563_Write_Add,写数据 
	IIC2_Wait_Ack();	   
  IIC2_Send_Byte(WriteAddr%256);   //发送低地址
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //发送字节							   
	IIC2_Wait_Ack();  		    	   
  IIC2_Stop();//产生一个停止条件 
  xDelay_ms(10);	 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//在PCF8563里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//ptChar:数据数组首地址
//Len        :要写入数据的长度2,4
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
//在PCF8563里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//ptChar:数据数组首地址
//Len        :要读出数据的长度2,4
unsigned char PCF8563_ReadLenByte(unsigned short int ReadAddr,unsigned char *ptChar,unsigned char Len)
{  	

	unsigned char t,*temp;
	temp = ptChar;          
	for(t=0;t<Len;t++)
	{
		*ptChar++=PCF8563_ReadOneByte(ReadAddr++);
	}
	
	//检测时间返回是否正确
	//当星期7时，为值为0
	if((temp[5]==0)||(temp[3]==0))
	{
		return 1;
	}
	return 0;
}

/////////////////////////十进制转BCD//////////////////////////////////////

unsigned char Dec_ToBcd(unsigned char Dec_Val)
{
	unsigned char Bcd_Val;
	Bcd_Val=(((Dec_Val/10)<<4)|(Dec_Val%10));
	return Bcd_Val;
}

///////////////////////////BCD转十进制////////////////////////////////////

unsigned char Bcd_ToDec(unsigned char Bcd_Val)
{
	unsigned char Dec_Val;
	Dec_Val=((((Bcd_Val&0xf0)>>4)*10)+(Bcd_Val&0x0f));
	return Dec_Val;
}

////////////////////初始化////////////////////////////
void PCF8563_RTC_Init(void)
{
	
	PCF8563_IIC_Init();
	PCF8563_start();	//时钟启动
	//第一次配置
	if (PCF8563_ReadOneByte(ALARM_MIN_REG) != 0x88)		//芯片是否已经配置过，使用分钟报警寄存器作为判断，中断关闭
	{	 			
 		PCF8563_WriteOneByte(ALARM_MIN_REG,0X88);//将数据写入PCF8563相应寄存器
		RTC_Set_F1(&Calendar);   //设置时间，年、月、日、时、分、秒。
	}  
//	RTC_Get();//更新时间
}


//////////////////////////////////////////RTC时间设置写入/////////////////////////////////////////////////////
void RTC_Set_F1(Calendar_obj *Calendar_info)
{
	 unsigned char b_time[7]={0};
	//8563使用的是BCD码，所以需要将十六进制转换成BCD码
	 (19==(Calendar_info->w_year/100))?(Calendar_info->w_month|=0x80):(Calendar_info->w_month&=~0x80);//判断世纪
	 b_time[6]=Dec_ToBcd((unsigned char)(Calendar_info->w_year%100));//0~99
	 b_time[5]=Dec_ToBcd(Calendar_info->w_month);//世纪（bit7）+月份  bit7=0表示为20xx,bit7=1表示19xx;
	 b_time[3]=Dec_ToBcd(Calendar_info->w_date);
	 if(7==Calc_Week(Calendar_info))//星期)////经基姆拉尔森算公式1~7表示星期一至星期日//PCF8563的星期顺序 0~6-->星期日~星期六
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
	 //读写数据方式
	 //1.每次读写完后，内嵌字地址寄存器都会自动增量偏移
	 //2.或逐地址读写
	 PCF8563_WriteOneByte(SEC_REG	,b_time[0]);
	 PCF8563_WriteOneByte(MIN_REG	,b_time[1]);
	 PCF8563_WriteOneByte(HOUR_REG,b_time[2]);
	 PCF8563_WriteOneByte(DAY_REG, b_time[3]);
	 PCF8563_WriteOneByte(WEEK_REG,b_time[4]);
	 PCF8563_WriteOneByte(MON_REG, b_time[5]);
	 PCF8563_WriteOneByte(YEAR_REG,b_time[6]);

}

//////////////////////////////////////////RTC时间设置写入（含星期形参）/////////////////////////////////////////////////////
void RTC_Set_F2(Calendar_obj *Calendar_info)
{
	//适应PCF8563的星期顺序 0~6-->星期日~星期六
	 unsigned char b_time[7]={0};
	//8563使用的是BCD码，所以需要将十六进制转换成BCD码
	 (19==(Calendar_info->w_year/100))?(Calendar_info->w_month|=0x80):(Calendar_info->w_month&=~0x80);//判断世纪
	 b_time[6]=Dec_ToBcd((unsigned char)(Calendar_info->w_year%100));//0~99
	 b_time[5]=Dec_ToBcd(Calendar_info->w_month);//世纪（bit7）+月份  bit7=0表示为20xx,bit7=1表示19xx;
	 b_time[3]=Dec_ToBcd(Calendar_info->w_date);
	 if(0==Calendar_info->w_week)//星期)
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
	 //读写数据方式
	 //1.每次读写完后，内嵌字地址寄存器都会自动增量偏移
	 //2.或逐地址读写
	 PCF8563_WriteOneByte(SEC_REG	,b_time[0]);
	 PCF8563_WriteOneByte(MIN_REG	,b_time[1]);
	 PCF8563_WriteOneByte(HOUR_REG,b_time[2]);
	 PCF8563_WriteOneByte(DAY_REG, b_time[3]);
	 PCF8563_WriteOneByte(WEEK_REG,b_time[4]);
	 PCF8563_WriteOneByte(MON_REG, b_time[5]);
	 PCF8563_WriteOneByte(YEAR_REG,b_time[6]);

}

/////////////////////////读RTC////////////////////////////////////////////////////
void RTC_Get(void)
{
    unsigned char temp[7]={0};
    temp[0]= PCF8563_ReadOneByte(SEC_REG)&0x7F;
		temp[1]= PCF8563_ReadOneByte(MIN_REG)&0x7F;
		temp[2]= PCF8563_ReadOneByte(HOUR_REG)&0x3F;
		temp[3]= PCF8563_ReadOneByte(DAY_REG)&0x3F;
		temp[4]= PCF8563_ReadOneByte(WEEK_REG)&0x07;//星期
		temp[5]= PCF8563_ReadOneByte(MON_REG)&0x1F;
		temp[6]= PCF8563_ReadOneByte(YEAR_REG);
		
	  Calendar.sec=Bcd_ToDec(temp[0]);
		Calendar.min=Bcd_ToDec(temp[1]);	
		Calendar.hour=Bcd_ToDec(temp[2]);
		Calendar.w_date=Bcd_ToDec(temp[3]);
		Calendar.w_week=Bcd_ToDec(temp[4]);
		Calendar.w_month=Bcd_ToDec(temp[5]);		
		Calendar.w_year=Bcd_ToDec(temp[6]);
		//经基姆拉尔森算公式1~7表示星期一至星期日//PCF8563的星期顺序 0~6-->星期日~星期六
		if(7==Calendar.w_week)   Calendar.w_week=0;	//星期协议顺序调整
		(0x80==(Calendar.w_month&0x80))?(Calendar.w_year+=1900):(Calendar.w_year+=2000);
		Calendar_UI(&Calendar);	//调用日历时间UI
}
/////////////////////////////////////计算星期///////////////////////////////////////////////////
unsigned char Calc_Week(Calendar_obj *Calendar_info)
{
	unsigned int weektxt;//星期变量
	unsigned short int virt_yeartxt;//创建一个虚拟适用于基姆拉尔森算式的年变量
	unsigned char virt_monthtxt;//创建一个虚拟适用于基姆拉尔森算式的月变量
	virt_yeartxt=Calendar_info->w_year;//赋予给 虚拟年
	virt_monthtxt=Calendar_info->w_month;//赋予给 虚拟月
	//------------------------------通过公历年月日求得星期------------------------------------------------------
	//把一月和二月看成是上一年的十三月和十四月，例：如果是2004-1-10则换算成：2003-13-10来代入公式计算。
	if((1==Calendar_info->w_month)||(2==Calendar_info->w_month))
	{		
		virt_monthtxt+=12; //产生 13/14
		virt_yeartxt-=1;//赋予给 虚拟年
	}
	//基姆拉尔森算式 此公式是 0~6表示星期一至星期日
	weektxt= (Calendar_info->w_date + 2*virt_monthtxt + 3*(virt_monthtxt+1)/5 + virt_yeartxt+virt_yeartxt/4 - virt_yeartxt/100 + virt_yeartxt/400)%7;
	weektxt+=1;//1~7表示星期一至星期日
	return (unsigned char)weektxt;
}



