

#ifndef __SLCD_H__
#define __SLCD_H__

#include "pindef.h"
#include "delay.h"

typedef enum
{
	CMD_MODE, //定义写命令模式
	DATA_MODE, //定义写数据模式  
}MODE_ENUM;

//OLED模式设置
#define SIZE 16
//#define SIZE 8    //SIZE选择英文字体的大小
#define XLevelL        0x00
#define XLevelH        0x10
#define Max_Column    128
#define Max_Row        64
#define Brightness    0xFF 
#define X_WIDTH     128
#define Y_WIDTH     64

extern const unsigned char F8X16[][16];
extern const unsigned char F14x14[][16];
extern const unsigned char Arduino_PIC48x48[];

extern const unsigned char PIC32x32[];
extern const unsigned char Default_Font16x16[][16];
extern const unsigned char Song_F16x16[][16];

void OLED_Init(void);
void OLED_Clear(void); 
void OLED_Display_Off(void); 
void OLED_Display_On(void);
void OLED_DrawLine(unsigned char start_x, unsigned char start_y, unsigned char end_x, unsigned char end_y);
void Gui_Rectangle(unsigned int x, unsigned int y, unsigned int l, unsigned int w);
void OLED_ShowChn(unsigned char x, unsigned char y, unsigned char chinese);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char *chr);  
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size);
void OLED_ShowBmp(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void GUI_F6x8(unsigned char x, unsigned char y, unsigned char chr,const unsigned char Font[][6]);
void GUI_F8x16(unsigned char x, unsigned char y, unsigned char chr);
void GUI_F14x14(unsigned char x, unsigned char y, unsigned char c);
void GUI_F16x16(unsigned char x, unsigned char y,unsigned char chn);
void GUI_F32x32(unsigned char x, unsigned char y, unsigned char font);
void GUI_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,const unsigned char BMP[]);
void LOGO_BMP(const unsigned char BMP[]);
#endif


