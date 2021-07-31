#ifndef __LCD_GUI_H__
#define __LCD_GUI_H__

unsigned int LCD_BGR2RGB(unsigned int c);
void Gui_Circle(unsigned int X,unsigned int Y,unsigned int R,unsigned int fc); 
void OLED_DrawLine(unsigned int x0, unsigned int y0,unsigned int x1, unsigned int y1,unsigned int Color);  
void Gui_box(unsigned int x, unsigned int y, unsigned int w, unsigned int h,unsigned int bc);
void Gui_box2(unsigned int x,unsigned int y,unsigned int w,unsigned int h, u8 mode);
void DisplayButtonDown(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void DisplayButtonUp(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void Gui_DrawFont_GBK16(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, u8 *s);
void Gui_DrawFont_GBK24(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, u8 *s);
void Gui_DrawFont_Num32(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, unsigned int num) ;

void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void Gui_Rectangle(unsigned int x, unsigned int y, unsigned int l, unsigned int w,unsigned int bc);
void Gui_DrawFont_Num14(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, unsigned int num);
void Gui_DrawIcon_Bat14(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, unsigned int num);
#endif


