#include "stm32f10x.h"
#include "lcd.h"
#include "lcd_gui.h"
#include "font.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
unsigned int LCD_BGR2RGB(unsigned int c)
{
  unsigned int  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void Gui_Circle(unsigned int X,unsigned int Y,unsigned int R,unsigned int fc) 
{//Bresenham�㷨 
    unsigned short  a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        OLED_DrawPoint(X+a,Y+b,fc);     //        7 
        OLED_DrawPoint(X-a,Y+b,fc);     //        6 
        OLED_DrawPoint(X+a,Y-b,fc);     //        2 
        OLED_DrawPoint(X-a,Y-b,fc);     //        3 
        OLED_DrawPoint(X+b,Y+a,fc);     //        8 
        OLED_DrawPoint(X-b,Y+a,fc);     //        5 
        OLED_DrawPoint(X+b,Y-a,fc);     //        1 
        OLED_DrawPoint(X-b,Y-a,fc);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        OLED_DrawPoint(X+a,Y+b,fc); 
        OLED_DrawPoint(X+a,Y+b,fc); 
        OLED_DrawPoint(X+a,Y-b,fc); 
        OLED_DrawPoint(X-a,Y-b,fc); 
        OLED_DrawPoint(X+b,Y+a,fc); 
        OLED_DrawPoint(X-b,Y+a,fc); 
        OLED_DrawPoint(X+b,Y-a,fc); 
        OLED_DrawPoint(X-b,Y-a,fc); 
    } 
	
} 
//******************************************************************
//��������  LCD_DrawLine
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    GUI����
//���������x1,y1:�������
//        	x2,y2:�յ����� 
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		OLED_DrawPoint(uRow,uCol,YELLOW);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

//******************************************************************
//��������  LCD_DrawRectangle
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    GUI������(�����)
//���������(x1,y1),(x2,y2):���εĶԽ�����
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************  
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}  

///////////////////////////////////////////////////////////////////////////////////////////////////////
//���ߺ�����ʹ��Bresenham �����㷨
void OLED_DrawLine(unsigned int x0, unsigned int y0,unsigned int x1, unsigned int y1,unsigned int Color)   
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	

	OLED_SetXY(x0,y0);
	dx = x1-x0;//����x����
	dy = y1-y0;//����y����

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x�������y���룬��ôÿ��x����ֻ��һ���㣬ÿ��y���������ɸ���
	{//���ߵĵ�������x���룬��x���������
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//Ҫ���ĵ������ᳬ��x����
		{
			//����
			OLED_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //�Ƿ���Ҫ����y����ֵ
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//����y����ֵ
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x����ֵÿ�λ���󶼵���1
		} // end for
	} // end if |slope| <= 1
	else//y�����x�ᣬ��ÿ��y����ֻ��һ���㣬x�����ɸ���
	{//��y��Ϊ��������
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			OLED_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void Gui_box(unsigned int x, unsigned int y, unsigned int w, unsigned int h,unsigned int bc)
{
	OLED_DrawLine(x,y,x+w,y,0xEF7D);
	OLED_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
	OLED_DrawLine(x,y+h,x+w,y+h,0x2965);
	OLED_DrawLine(x,y,x,y+h,0xEF7D);
  OLED_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void Gui_box2(unsigned int x,unsigned int y,unsigned int w,unsigned int h, u8 mode)
{
	if (mode==0)	{
		OLED_DrawLine(x,y,x+w,y,0xEF7D);
		OLED_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
		OLED_DrawLine(x,y+h,x+w,y+h,0x2965);
		OLED_DrawLine(x,y,x,y+h,0xEF7D);
		}
	if (mode==1)	{
		OLED_DrawLine(x,y,x+w,y,0x2965);
		OLED_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
		OLED_DrawLine(x,y+h,x+w,y+h,0xEF7D);
		OLED_DrawLine(x,y,x,y+h,0x2965);
	}
	if (mode==2)	{
		OLED_DrawLine(x,y,x+w,y,0xffff);
		OLED_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
		OLED_DrawLine(x,y+h,x+w,y+h,0xffff);
		OLED_DrawLine(x,y,x,y+h,0xffff);
	}
}
void Gui_Rectangle(unsigned int x, unsigned int y, unsigned int l, unsigned int w,unsigned int bc)//hua jindu tiao
{
	static unsigned char dx=0,dy=0;
	OLED_SetXY(x,y);
	for(dy=0;dy<w;dy++)
	for(dx=0;dx<l;dx++)
	{
	  OLED_DrawPoint(x+dx,y+dy,bc);//�������߼�
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************
��������: ����Ļ��ʾһ͹��İ�ť��
��    ��: unsigned int x1,y1,x2,y2 ��ť�����ϽǺ����½�����
**************************************************************************************/
void DisplayButtonDown(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
//	OLED_DrawLine(x1,  y1,  x2,y1, GRAY2);  //H
  OLED_DrawLine(x1,  y1,  x2,y1, BLUE);  //H
	OLED_DrawLine(x1+1,y1+1,x2,y1+1, GRAY1);  //H
	OLED_DrawLine(x1,  y1,  x1,y2, BLUE);  //V	
//	OLED_DrawLine(x1,  y1,  x1,y2, GRAY2);  //V
	OLED_DrawLine(x1+1,y1+1,x1+1,y2, GRAY1);  //V
	OLED_DrawLine(x1,  y2,  x2,y2, WHITE);  //H
	OLED_DrawLine(x2,  y1,  x2,y2, WHITE);  //V
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************
��������: ����Ļ��ʾһ���µİ�ť��
��    ��: unsigned int x1,y1,x2,y2 ��ť�����ϽǺ����½�����
**************************************************************************************/
void DisplayButtonUp(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	OLED_DrawLine(x1,  y1,  x2,y1, WHITE); //H
	OLED_DrawLine(x1,  y1,  x1,y2, WHITE); //V
	
	OLED_DrawLine(x1+1,y2-1,x2,y2-1, GRAY1);  //H
	OLED_DrawLine(x1,  y2,  x2,y2, GRAY2);  //H
	OLED_DrawLine(x2-1,y1+1,x2-1,y2, GRAY1);  //V
  OLED_DrawLine(x2  ,y1  ,x2,y2, GRAY2); //V
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void Gui_DrawFont_GBK16(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, u8 *s)
{
	unsigned int i,j;
	unsigned short k,x0;
	x0=x;
	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				(k>32)?( k-=32):(k=0); 
				for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
				{
				 if(asc16[k*16+i]&(0x80>>j))	
				 {
					 OLED_DrawPoint(x+j,y+i,fc);
				 }
				 else 
				 {
					 if (fc!=bc) 
					 {
						 OLED_DrawPoint(x+j,y+i,bc);//���������ɫ�뱳����ɫ��һ��
					 }
				 }
				}
				x+=8;
			}
			s++;
		}			
		else 
		{
			for (k=0;k<hz16_num;k++) 
			{
				 if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
				 { 
					for(i=0;i<16;i++)
					{
						for(j=0;j<8;j++) 
						{
							if(hz16[k].Msk[i*2]&(0x80>>j))	OLED_DrawPoint(x+j,y+i,fc);
							else 
							{
								if (fc!=bc) OLED_DrawPoint(x+j,y+i,bc);
							}
						}
						for(j=0;j<8;j++) 
						{
							if(hz16[k].Msk[i*2+1]&(0x80>>j))	OLED_DrawPoint(x+j+8,y+i,fc);
							else 
							{
								if (fc!=bc) OLED_DrawPoint(x+j+8,y+i,bc);
							}
						}
				 }
			 }
	   }
		s+=2;x+=16;
	 } 	
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void Gui_DrawFont_GBK24(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, u8 *s)
{
	unsigned int i,j;
	unsigned short k;

	while(*s) 
	{
		if(*s<0x80) 
		{
			k=*s;
			(k>32)?( k-=32):(k=0); 
		  for(i=0;i<16;i++)
			for(j=0;j<8;j++) 
			{
				if(asc16[k*16+i]&(0x80>>j))	
				{
				   OLED_DrawPoint(x+j,y+i,fc);
				}
				else 
				{
					if (fc!=bc) 
					{
						OLED_DrawPoint(x+j,y+i,bc);
					}
				}
			}
			s++;x+=8;
		}
		else 
		{

			for (k=0;k<hz24_num;k++) 
			{
			  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<24;i++)
				    {
							for(j=0;j<8;j++) 
							{
									if(hz24[k].Msk[i*3]&(0x80>>j))
									{
										OLED_DrawPoint(x+j,y+i,fc);
									}
									else 
									{
										if (fc!=bc) 
										{
											OLED_DrawPoint(x+j,y+i,bc);
										}
									}
							}
						  for(j=0;j<8;j++) 
							{
						    if(hz24[k].Msk[i*3+1]&(0x80>>j))	
								{
									OLED_DrawPoint(x+j+8,y+i,fc);
								}
								else 
								{
									if (fc!=bc) 
									{
										OLED_DrawPoint(x+j+8,y+i,bc);
									}
								}
							}
						  for(j=0;j<8;j++) 
							{
						    if(hz24[k].Msk[i*3+2]&(0x80>>j))
								{									
								  OLED_DrawPoint(x+j+16,y+i,fc);
								}
								else 
								{
									if (fc!=bc) 
									{
										OLED_DrawPoint(x+j+16,y+i,bc);
									}
								}
							}
				    }
			  }
			}
			s+=2;x+=24;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void Gui_DrawFont_Num32(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, unsigned int num)
{
	unsigned int i,j,k,c;
	//lcd_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
//	w=w/8;

    for(i=0;i<32;i++)
    {
      for(j=0;j<4;j++) 
      {
      	c=*(sz32+num*32*4+i*4+j);
				for (k=0;k<8;k++)	
				{
					if(c&(0x80>>k))
					{
						OLED_DrawPoint(x+j*8+k,y+i,fc);
					}
					else 
					{
					 if (fc!=bc) 
					 {
						 OLED_DrawPoint(x+j*8+k,y+i,bc);
					 }
					}
				}
      }
    }
}

#define CONSOLAS_FONT_COL 14
#define CONSOLAS_FONT_ROW 1

void Gui_DrawFont_Num14(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, unsigned int num)
{
	  unsigned int i,j,k,c;
    for(i=0;i<CONSOLAS_FONT_COL;i++)//ɨ��14��
    {
      for(j=0;j<CONSOLAS_FONT_ROW;j++) //
      {
      	c=*(consolas_num14+num*CONSOLAS_FONT_COL*CONSOLAS_FONT_ROW+i*CONSOLAS_FONT_ROW+j);
				for (k=0;k<8;k++)	
				{
					if(c&(0x80>>k))
					{
						OLED_DrawPoint(x+j*8+k,y+i,fc);
					}
					else 
					{
					 if (fc!=bc) 
					 {
						 OLED_DrawPoint(x+j*8+k,y+i,bc);
					 }
					}
				}
      }
    }	
}
#define BAT_IMAG_COL 14
#define BAT_IMAG_ROW 2

void Gui_DrawIcon_Bat14(unsigned int x, unsigned int y, unsigned int fc, unsigned int bc, unsigned int num)
{
	  unsigned int i,j,k,c;
    for(i=0;i<BAT_IMAG_COL;i++)//ɨ��14��
    {
      for(j=0;j<BAT_IMAG_ROW;j++) //
      {
      	c=*(bat14+num*BAT_IMAG_COL*BAT_IMAG_ROW+i*BAT_IMAG_ROW+j);
				for (k=0;k<8;k++)	
				{
					if(c&(0x80>>k))
					{
						OLED_DrawPoint(x+j*8+k,y+i,fc);
					}
					else 
					{
					 if (fc!=bc) 
					 {
						 OLED_DrawPoint(x+j*8+k,y+i,bc);
					 }
					}
				}
      }
    }	
}	

