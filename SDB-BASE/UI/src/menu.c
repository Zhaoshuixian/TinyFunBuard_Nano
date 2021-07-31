
#include "slcd.h"
#include "ir.h"
#include "rrs.h"
#include "ntc.h"
#include "clock.h"
#include "esp8266.h"
#include <string.h>
#include <stdio.h>
#include "menu.h"
typedef struct
{
	unsigned char pnext;
	unsigned int rtime;
}Menu_st;

Menu_st Menu;

/////////////////////////////////////////////////////////////////////////////////////////////////
void Temperature_UI(Environ_sensor *Temperature_Info)
{
	 GUI_F32x32(70,4,Temperature_Info->value/10);
	 GUI_F32x32(88,4,Temperature_Info->value%10);
	 GUI_F32x32(106,4,12);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void Calendar_UI(Calendar_obj *Calendar_Info)
{
	 unsigned char CLOCK_BUFF[20]={0};
	 snprintf((char *)CLOCK_BUFF,18,"%d-%02d-%02d %02d:%02d",Calendar_Info->year,Calendar_Info->month,Calendar_Info->day,Calendar_Info->hour,Calendar_Info->min);
	 if(0==Calendar_Info->sec%2)
	 {
		 CLOCK_BUFF[13]=':';
	 }
	 else
	 {
		 CLOCK_BUFF[13]=' ';
	 }
	 OLED_ShowStr(0,0,(unsigned char*)CLOCK_BUFF);	 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Ir_UI(ir_st *ir_info)
{
	 unsigned char CODE_BUFF[40]={0};
	 snprintf((char *)CODE_BUFF,36,"%02X-%02X-%02X-%02X",ir_info->data.addr,ir_info->data.addrn,ir_info->data.cmd,ir_info->data.cmdn);	
	 GUI_F16x16(12,6,4);
	 GUI_F16x16(26,6,5);
	 GUI_F16x16(42,6,6); 
	 GUI_F16x16(58,6,7); 
	 GUI_F16x16(74,6,8); 
	 GUI_F16x16(90,6,9); 
	 GUI_F16x16(104,6,9); 
	 GUI_F16x16(120,6,9); 	
	 OLED_ShowStr(16,30,(unsigned char*)CODE_BUFF);		//IR-CODE	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Menu_Thread(void)
{
	static unsigned char ui_cflag;
	Calendar_UI(&Calendar);	      //调用日历时间UI	
	if(PWR_SW==ir.valid_data)
	{
		Menu.pnext=1;
	}
	if(MENU==ir.valid_data)
	{
	
	}
	if(TEST==ir.valid_data)
	{
		Menu.pnext=2;
		Menu.rtime=300;
	}
	switch(Menu.pnext)
	{
		case 0:
			break;
		case 1:
			if(ui_cflag)
			{
			  OLED_Clear();
			  ui_cflag=0;
			}	
		  LOGO_BMP(Arduino_PIC48x48);
		  Temperature_UI(&Temperature);				
			break;
		case 2:
			if(0<(--Menu.rtime/MENU_TASK_TIME))
			{
				if(0==ui_cflag)
				{
					 OLED_Clear();
					 ui_cflag=1;
				}				
				Ir_UI(&ir);
			}
			else
			{
				Menu.pnext=1;
				ui_cflag=1;			
			}
			break;
		default:
			break;
	}

}
