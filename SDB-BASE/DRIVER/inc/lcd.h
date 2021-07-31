#ifndef __LCD_H__
#define __LCD_H__

#include "stm32f10x_conf.h"
#include "pindef.h"


#define RED   	0xf800
#define GREEN 	0x07e0
#define BLUE   	0x001f
#define WHITE  	0xffff
#define BLACK	  0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410    //灰色1 00000 000000 00000
#define GRAY2   0x4208    //灰色2 1111111111011111


//液晶控制口电平操作语句宏定义
#ifndef USE_096OLED
#define	OLED_CS(x)    (x==0?GPIO_ResetBits(OLED_PORT_A,OLED_CS_PIN):GPIO_SetBits(OLED_PORT_A,OLED_CS_PIN)) 
#define	OLED_SCL(x)   (x==0?GPIO_ResetBits(OLED_PORT_A,OLED_SCL_PIN):GPIO_SetBits(OLED_PORT_A,OLED_SCL_PIN)) 
#define	OLED_SDA(x)   (x==0?GPIO_ResetBits(OLED_PORT_A,OLED_SDA_PIN):GPIO_SetBits(OLED_PORT_A,OLED_SDA_PIN)) 
#define	OLED_A0(x)    (x==0?GPIO_ResetBits(OLED_PORT_B,OLED_A0_PIN):GPIO_SetBits(OLED_PORT_B,OLED_A0_PIN)) 
#endif
void OLED_WriteIndex(char Index);
void OLED_WriteData(char Data);   
void OLED_WriteReg(char Index,char Data);
unsigned int OLED_ReadReg(char LCD_Reg);
void OLED_Reset(void);
void OLED_Init(void);
void OLED_Clear(unsigned int Color);
void OLED_SetXY(unsigned int x,unsigned int y);
void OLED_DrawPoint(unsigned int x,unsigned int y,unsigned int Data);
unsigned int OLED_ReadPoint(unsigned int x,unsigned int y);
void OLED_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end);
void OLED_WriteData_16Bit(unsigned int Data);
void OLED_Clear_SpeciatArea(unsigned int x,unsigned y,unsigned int l,unsigned int w,unsigned int bc);

#endif

