

#include "ui.h"


const char* wk_p[7]={"Sun","Mon","Tus","Wed","Thr","Fri","Sat"};//适应PCF8563的星期顺序 0~6-->星期日~星期六
unsigned int  SEC_CLOR=BLACK;

//////////////////////////////////////////////////////////////////////////////////////
void Home_Init(void)
{
//////////////////////////////////////////////////////////////////////////////////////
		LCD_DrawRectangle(1,1,127,127);	//画矩形 
		LCD_DrawLine(1,20,127,20);
		LCD_DrawLine(1,107,127,107);	
}

//////////////////////////////////////////////////////////////////////////////////////
void Calendar_UI(Calendar_obj *Calendar_info)
{
	#define CALENDAR_X_POS 10 //
	#define CALENDAR_X_INC 8
	#define CALENDAR_Y_POS 84
	#define CALENDAR_Y_INC 0
	
	#define TIME_X_POS 73
	#define	TIME_X_INC 7
	#define TIME_Y_POS 4
	#define	TIME_Y_INC 0	
	//YYYY-MM-DD WEEK
	(Calendar_info->sec%2)?(SEC_CLOR=RED):(SEC_CLOR=BLACK);
	Gui_DrawFont_Num14(CALENDAR_X_POS,CALENDAR_Y_POS,YELLOW,BLACK,Calendar_info->w_year/1000);
	Gui_DrawFont_Num14(CALENDAR_X_POS+CALENDAR_X_INC,CALENDAR_Y_POS,YELLOW,BLACK,Calendar_info->w_year/100%10);
	Gui_DrawFont_Num14(CALENDAR_X_POS+CALENDAR_X_INC*2,CALENDAR_Y_POS,YELLOW,BLACK,Calendar_info->w_year%100/10);
	Gui_DrawFont_Num14(CALENDAR_X_POS+CALENDAR_X_INC*3,CALENDAR_Y_POS,YELLOW,BLACK,Calendar_info->w_year%100%10);
	
	Gui_DrawFont_Num14(CALENDAR_X_POS+CALENDAR_X_INC*4,CALENDAR_Y_POS,YELLOW,BLACK,12);//-
	
	Gui_DrawFont_Num14(CALENDAR_X_POS+CALENDAR_X_INC*5,CALENDAR_Y_POS,YELLOW,BLACK,Calendar_info->w_month/10);
	Gui_DrawFont_Num14(CALENDAR_X_POS+CALENDAR_X_INC*6,CALENDAR_Y_POS,YELLOW,BLACK,Calendar_info->w_month%10);
	
	Gui_DrawFont_Num14(CALENDAR_X_POS+CALENDAR_X_INC*7,CALENDAR_Y_POS,YELLOW,BLACK,12);//-
	
	Gui_DrawFont_Num14(CALENDAR_X_POS+CALENDAR_X_INC*8,CALENDAR_Y_POS,YELLOW,BLACK,Calendar_info->w_date/10);
	Gui_DrawFont_Num14(CALENDAR_X_POS+CALENDAR_X_INC*9,CALENDAR_Y_POS,YELLOW,BLACK,Calendar_info->w_date%10);

	Gui_DrawFont_GBK16(100,CALENDAR_Y_POS,YELLOW,BLACK,(unsigned char *)wk_p[Calendar_info->w_week]);	
	
	Gui_DrawFont_Num14(TIME_X_POS+TIME_X_INC*0,TIME_Y_POS,RED,BLACK,Calendar_info->hour/10);
	Gui_DrawFont_Num14(TIME_X_POS+TIME_X_INC*1,TIME_Y_POS,RED,BLACK,Calendar_info->hour%10);
	Gui_DrawFont_Num14(TIME_X_POS+TIME_X_INC*2,TIME_Y_POS,SEC_CLOR,BLACK,10);
	Gui_DrawFont_Num14(TIME_X_POS+TIME_X_INC*3,TIME_Y_POS,RED,BLACK,Calendar_info->min/10);
	Gui_DrawFont_Num14(TIME_X_POS+TIME_X_INC*4,TIME_Y_POS,RED,BLACK,Calendar_info->min%10);		
}

//////////////////////////////////////////////////////////////////////////////////////
void Temperature_UI(Environ_sensor *Temperature_info)
{
	#define TEMP_X_POS 3
	#define TEMP_X_INC 16
	#define TEMP_Y_POS 38
	#define TEMP_Y_INC 0	
	
	#define TEMP_VAL_X_POS (TEMP_X_POS+TEMP_X_INC*2+6)
	#define TEMP_VAL_X_INC 26
	if(1==Temperature_info->online)
	{
		Gui_DrawFont_GBK16(TEMP_X_POS+TEMP_X_INC*0,TEMP_Y_POS,RED,BLACK,(unsigned char *)"温度:");
		Gui_DrawFont_Num32(TEMP_VAL_X_POS+TEMP_VAL_X_INC*0,TEMP_Y_POS,WHITE,BLACK,Temperature_info->value/10);
		Gui_DrawFont_Num32(TEMP_VAL_X_POS+TEMP_VAL_X_INC*1,TEMP_Y_POS,WHITE,BLACK,Temperature_info->value%10);
		Gui_DrawFont_Num32(TEMP_VAL_X_POS+TEMP_VAL_X_INC*2+2,TEMP_Y_POS,RED,BLACK,13);	//
	}
	else
	{
		
	}
}

//////////////////////////////////////////////////////////////////////////////////////
void Battery_UI(Battery_obj *Battery_info)//电池数据
{
	#define BAT_X_POS 111
	#define BAT_Y_POS 4
	if(CHARGE==Battery_info->sta)
	{
	  Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,WHITE,BLACK,10);//Batter Charge Icon
	}
	else
	{
		switch(Battery_info->pwr)
		{
			case PER_10:
				Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,RED,BLACK,0);//Batter 10%
				break;
			case PER_20:
				Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,WHITE,BLACK,1);//Batter 20%
				break;
			case PER_30:
				Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,WHITE,BLACK,2);//Batter 30%
				break;
			case PER_40:
				Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,WHITE,BLACK,3);//Batter 40%
				break;
			case PER_50:
				Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,WHITE,BLACK,4);//Battery
				break;
			case PER_60:
				Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,WHITE,BLACK,5);//Battery 
				break;
			case PER_70:
				Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,WHITE,BLACK,6);//Battery 
				break;
			case PER_80:
				Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,WHITE,BLACK,7);//Battery 
				break;
			case PER_90:
				Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,WHITE,BLACK,8);//Battery 
				break;
			case PER_100:
				Gui_DrawIcon_Bat14(BAT_X_POS,BAT_Y_POS,WHITE,BLACK,9);//Battery 
				break;	
      default:break;			
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
void Bluetooth_UI(Bluetooth_obj *Bluetooth_info)
{
	#define BT_X_POS 35
	#define BT_Y_POS 4
	static unsigned char i=0;	
	if(CONNECT==Bluetooth_info->sta)//connect status
	{
		i=0;
		Gui_DrawIcon_Bat14(BT_X_POS,BT_Y_POS,BLUE,BLACK,16);//
	}
	else if(TRANSFER==Bluetooth_info->sta)
	{
		i=0;
		Gui_DrawIcon_Bat14(BT_X_POS,BT_Y_POS,BLUE,BLACK,17);//bt Icon
//		Gui_Rectangle(3,110,Bluetooth_info->Transfer_Val,15,BLUE);//			
	}
	else//unconnect
	{
		if(0==i)
		{
			i=1;
			OLED_Clear_SpeciatArea(BT_X_POS,BT_Y_POS,22,14,BLACK);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
void ProgressBar_UI(ProgressBar_obj *ProgressBar_info)
{
	#define PROGB_X_POS 2
	#define PROGB_Y_POS 110
	#define PROGB_X_INC 7
	
	#define PROGB_VAL_X_POS (PROGB_X_POS+94)
	static unsigned char i=0,j=0;
	if(USING==ProgressBar_info->use_flag)
	{
			Gui_Rectangle(PROGB_X_POS+PROGB_X_INC*0,PROGB_Y_POS,ProgressBar_info->value,15,BLUE);//
			if(100>ProgressBar_info->value)//not full
			{
				i=0;
				Gui_DrawFont_Num14(PROGB_VAL_X_POS+PROGB_X_INC*1,PROGB_Y_POS,WHITE,BLACK,ProgressBar_info->value/10);//
				Gui_DrawFont_Num14(PROGB_VAL_X_POS+PROGB_X_INC*2,PROGB_Y_POS,WHITE,BLACK,ProgressBar_info->value%10);	
				Gui_DrawFont_Num14(PROGB_VAL_X_POS+PROGB_X_INC*3+1,PROGB_Y_POS,WHITE,BLACK,11);//%	
			}
      if(100==ProgressBar_info->value)//full
			{
				if(0==i)
				{
					i=1;
			  	OLED_Clear_SpeciatArea(PROGB_X_POS+100,PROGB_Y_POS,22,14,BLACK);
				}
				Gui_DrawFont_Num14(PROGB_VAL_X_POS+PROGB_X_INC*1+6,PROGB_Y_POS,WHITE,BLACK,15);
				Gui_DrawFont_Num14(PROGB_VAL_X_POS+PROGB_X_INC*2+6,PROGB_Y_POS,WHITE,BLACK,16);//"OK"
			}	
	}
	else
	{
		if(0==j)
		{
			j=1;
			OLED_Clear_SpeciatArea(PROGB_X_POS,PROGB_Y_POS,124,14,BLACK);
		}		
	}
}

//////////////////////////////////////////////////////////////////////////////////////
void Alarm_UI(Alarm_obj *Alarm_info)
{
	#define ALARM_X_POS 52
	#define ALARM_Y_POS 4
  static unsigned char i=0;	
	if(OPEN==Alarm_info->sta)
	{
		 i=0;
     Gui_DrawIcon_Bat14(ALARM_X_POS,ALARM_Y_POS,WHITE,BLACK,18);//alarm Icon		
	}
	else
	{
		if(0==i)
		{
			i=1;
			OLED_Clear_SpeciatArea(ALARM_X_POS,ALARM_Y_POS,14,14,BLACK);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////
void Internet_UI(Internet_obj *Internet_info)
{
	#define INTERNET_X_POS 3
	#define INTERNET_Y_POS 4	
	static unsigned char i=0;	
	if(Internet_info->use_flag)
	{
		i=0;
		if(CON==Internet_info->sta)	
		{
			Gui_DrawIcon_Bat14(INTERNET_X_POS,INTERNET_Y_POS,WHITE,BLACK,19);//Internet Icon
		}
		else
		{
			Gui_DrawIcon_Bat14(INTERNET_X_POS,INTERNET_Y_POS,WHITE,BLACK,20);//Internet Icon
		}
  }
	else
	{
		if(0==i)
		{
			i=1;
			OLED_Clear_SpeciatArea(INTERNET_X_POS,INTERNET_Y_POS,14,14,BLACK);
		}		
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////
void GSM_UI()
{
	#define GSM_X_POS 3
	#define GSM_Y_POS 4		
	Gui_DrawIcon_Bat14(GSM_X_POS,GSM_Y_POS,WHITE,BLACK,14);//GSM Icon	
}

//////////////////////////////////////////////////////////////////////////////////////
void WiFi_UI()
{
	#define WIFI_X_POS 20
	#define WIFI_Y_POS 4	
  Gui_DrawIcon_Bat14(WIFI_X_POS,WIFI_Y_POS,WHITE,BLACK,14);//GSM Icon		
}

