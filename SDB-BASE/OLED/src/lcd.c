

#include "lcd.h"
#include "delay.h"
#include "bsp.h"


/////////////////////////////////////////////////////////////////////////
void  SPI_WriteData(char Data)
{
	unsigned int i=0;
	for(i=0;i<8;i++)
	{
	  ((Data<<i)&0x80)?(OLED_SDA(1)):(OLED_SDA(0));
		OLED_SCL(0);       
		OLED_SCL(1); 				
	}
}

/////////////////////////////////////////////////////////////////////////
void OLED_WriteIndex(char Index)//��Һ����дһ��8λָ��
{
   OLED_CS(0);
   OLED_A0(0);
   SPI_WriteData(Index);
   OLED_CS(1);
}

/////////////////////////////////////////////////////////////////////////
void OLED_WriteData(char Data)
{
   OLED_CS(0);
   OLED_A0(1);//A0=1--����;
   SPI_WriteData(Data);
   OLED_CS(1); 
}

/////////////////////////////////////////////////////////////////////////
void OLED_WriteData_16Bit(unsigned int Data)
{
   OLED_CS(0);
   OLED_A0(1);//A0=1--����;
   SPI_WriteData(Data>>8); 	//д���8λ����
   SPI_WriteData(Data); 	//д���8λ����
   OLED_CS(1); 
}

/////////////////////////////////////////////////////////////////////////
void OLED_WriteReg(char Index,char Data)
{
   OLED_WriteIndex(Index);
   OLED_WriteData(Data);
}

/////////////////////////////////////////////////////////////////////////
void OLED_Reset(void)
{
//	OLED_RST(0);
//	xDelay_ms(100);
//	OLED_RST(1);
//	xDelay_ms(50);
}

/////////////////////////////////////////////////////////////////////////
void OLED_Init(void)
{	
	OLED_GPIO_Init();
	OLED_Reset(); //Reset before LCD Init.
	OLED_WriteIndex(0x11);//Sleep exit 
//  xDelay_ms(120);	
	//ST7735R Frame Rate
	OLED_WriteIndex(0xB1); 
	OLED_WriteData(0x01); 
	OLED_WriteData(0x2C); 
	OLED_WriteData(0x2D); 
	OLED_WriteIndex(0xB2); 
	OLED_WriteData(0x01); 
	OLED_WriteData(0x2C); 
	OLED_WriteData(0x2D); 
	OLED_WriteIndex(0xB3); 
	OLED_WriteData(0x01); 
	OLED_WriteData(0x2C); 
	OLED_WriteData(0x2D); 
	OLED_WriteData(0x01); 
	OLED_WriteData(0x2C); 
	OLED_WriteData(0x2D); 	
	OLED_WriteIndex(0xB4); //Column inversion 
	OLED_WriteData(0x07); 	
	//ST7735R Power Sequence
	OLED_WriteIndex(0xC0); 
	OLED_WriteData(0xA2); 
	OLED_WriteData(0x02); 
	OLED_WriteData(0x84); 
	OLED_WriteIndex(0xC1); 
	OLED_WriteData(0xC5); 
	OLED_WriteIndex(0xC2); 
	OLED_WriteData(0x0A); 
	OLED_WriteData(0x00); 
	OLED_WriteIndex(0xC3); 
	OLED_WriteData(0x8A); 
	OLED_WriteData(0x2A); 
	OLED_WriteIndex(0xC4); 
	OLED_WriteData(0x8A); 
	OLED_WriteData(0xEE); 
	OLED_WriteIndex(0xC5); //VCOM 
	OLED_WriteData(0x0E); 
	OLED_WriteIndex(0x36); //MX, MY, RGB mode 
	OLED_WriteData(0xC8); 
	//ST7735R Gamma Sequence
	OLED_WriteIndex(0xe0); 
	OLED_WriteData(0x0f); 
	OLED_WriteData(0x1a); 
	OLED_WriteData(0x0f); 
	OLED_WriteData(0x18); 
	OLED_WriteData(0x2f); 
	OLED_WriteData(0x28); 
	OLED_WriteData(0x20); 
	OLED_WriteData(0x22); 
	OLED_WriteData(0x1f); 
	OLED_WriteData(0x1b); 
	OLED_WriteData(0x23); 
	OLED_WriteData(0x37); 
	OLED_WriteData(0x00); 	
	OLED_WriteData(0x07); 
	OLED_WriteData(0x02); 
	OLED_WriteData(0x10); 
	OLED_WriteIndex(0xe1); 
	OLED_WriteData(0x0f); 
	OLED_WriteData(0x1b); 
	OLED_WriteData(0x0f); 
	OLED_WriteData(0x17); 
	OLED_WriteData(0x33); 
	OLED_WriteData(0x2c); 
	OLED_WriteData(0x29); 
	OLED_WriteData(0x2e); 
	OLED_WriteData(0x30); 
	OLED_WriteData(0x30); 
	OLED_WriteData(0x39); 
	OLED_WriteData(0x3f); 
	OLED_WriteData(0x00); 
	OLED_WriteData(0x07); 
	OLED_WriteData(0x03); 
	OLED_WriteData(0x10);  
	OLED_WriteIndex(0x2a);
	OLED_WriteData(0x00);
	OLED_WriteData(0x00);
	OLED_WriteData(0x00);
	OLED_WriteData(0x7f);
	OLED_WriteIndex(0x2b);
	OLED_WriteData(0x00);
	OLED_WriteData(0x00);
	OLED_WriteData(0x00);
	OLED_WriteData(0x9f);
	OLED_WriteIndex(0xF0); //Enable test command  
	OLED_WriteData(0x01); 
	OLED_WriteIndex(0xF6); //Disable ram power save mode 
	OLED_WriteData(0x00); 
	OLED_WriteIndex(0x3A); //65k mode 
	OLED_WriteData(0x05); 
	OLED_WriteIndex(0x29);//Display on	
  OLED_Clear(BLACK);
//  xDelay_ms(10);
}


/////////////////////////////////////////////////////////////////////////
/*************************************************
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
*************************************************/
void OLED_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{		
	OLED_WriteIndex(0x2a);
	OLED_WriteData(0x00);
	OLED_WriteData(x_start+2);
	OLED_WriteData(0x00);
	OLED_WriteData(x_end+2);
	OLED_WriteIndex(0x2b);
	OLED_WriteData(0x00);
	OLED_WriteData(y_start+3);
	OLED_WriteData(0x00);
	OLED_WriteData(y_end+3);
	OLED_WriteIndex(0x2c);

}

/////////////////////////////////////////////////////////////////////////
/*************************************************
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
*************************************************/
void OLED_SetXY(unsigned int x,unsigned int y)
{
  	OLED_SetRegion(x,y,x,y);
}

/////////////////////////////////////////////////////////////////////////	
/*************************************************
���ܣ���һ����
��ڲ�������
*************************************************/
void OLED_DrawPoint(unsigned int x,unsigned int y,unsigned int Data)
{
//	OLED_SetRegion(x,y,x+1,y+1);
		OLED_SetRegion(x,y,x,y);
	OLED_WriteData_16Bit(Data);
}    

/////////////////////////////////////////////////////////////////////////
/*****************************************
 �������ܣ���TFTĳһ�����ɫ                          
 ���ڲ�����color  ����ɫֵ                                 
******************************************/
//unsigned int OLED_ReadPoint(unsigned int x,unsigned int y)
//{
//  unsigned int Data;
//  OLED_SetXY(x,y);

// // OLED_ReadData();//���������ֽ�
//  Data=OLED_ReadData();
//  OLED_WriteData(Data);
//  return Data;
//}
/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/

#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        128


void OLED_Clear(unsigned int Color)               
{	
   unsigned int i,m;
   OLED_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   OLED_WriteIndex(0x2C);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	OLED_WriteData_16Bit(Color);
    }   
}

void OLED_Clear_SpeciatArea(unsigned int x,unsigned y,unsigned int l,unsigned int w,unsigned int bc)
{
   unsigned int i,m;
//	 OLED_SetXY(x,y);
	 OLED_SetRegion(x,y,x+l,y+w);//sx-sy->ex-ey
   OLED_WriteIndex(0x2C);
   for(i=0;i<w;i++)
    for(m=0;m<l;m++)
    {	
	  	OLED_WriteData_16Bit(bc);
    }  	
}



