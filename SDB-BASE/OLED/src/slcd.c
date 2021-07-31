

#include "slcd.h"
#include "font.h"
#include "bsp.h"

#define SPI_MODE
/*
SPI模式下，RST需要有，否则不能正常显示
*/

//void IIC_Write(unsigned char wdata,MODE_ENUM Mode)
//{
//	IIC3_Start();
//	IIC3_Send_Byte(0x78);
//	if(DATA_MODE==Mode)
//	{
//	  IIC3_Send_Byte(0x40);			//write data
//	}
//	if(CMD_MODE==Mode)
//	{
//	  IIC3_Send_Byte(0x00);			//write command		
//	}
//	IIC3_Send_Byte(wdata);
//	IIC3_Stop();
//}
//////////////////////////////////////////////////////////////////////////////
void SPI_Write(unsigned char Data,MODE_ENUM Mode)
{
	unsigned char i;
	if(CMD_MODE==Mode)
	{
		OLED_DC(0);
	}
	if(DATA_MODE==Mode)
	{
		OLED_DC(1);
	}
	OLED_SCL(0);
		xDelay_us(50); 
//  Mode?(OLED_DC(1)):(OLED_DC(0));
	for(i=0;i<8;i++)
	{
    if(Data&0x80)
		{
			OLED_SDA(1);
		}
		else
		{
			OLED_SDA(0);
		}
		OLED_SCL(0);
		xDelay_us(50); 
		OLED_SCL(1);
		Data<<=1;
	}
//   OLED_DC(1);
}

void OLED_WriteData(unsigned char Data,MODE_ENUM Mode)
{
	#ifdef  SPI_MODE
	SPI_Write(Data,Mode);
	#else
	IIC_Write(Data,Mode);
	#endif
}


//////////////////////////////////////////////////////////////////////////////
static void OLED_SetXY(unsigned char x, unsigned char y)  
{      
    OLED_WriteData((0xb0+y),CMD_MODE); 
    OLED_WriteData((((x&0xf0)>>4)|0x10),CMD_MODE);//高4位  
    OLED_WriteData((x & 0x0f)|0x01,CMD_MODE);//低4位  
}

//////////////////////////////////////////////////////////////////////////////
//清屏，一开始这里写错了，把写命令写成了写数据，导致清屏不正确，发现屏幕上有很多噪点，说明没有清屏成功。
void OLED_Clear(void)  
{  
	unsigned char i,j;  

	for(i=0;i<8;i++)  
	{  
		OLED_WriteData(0xb0+i,CMD_MODE);
		OLED_WriteData(0x00,CMD_MODE);
		OLED_WriteData(0x10,CMD_MODE);
		for(j = 0; j < 128; j++)  
		{  
			OLED_WriteData(0x00, DATA_MODE);  
		}  
	}  
}  

//////////////////////////////////////////////////////////////////////////////
void OLED_Display_Off(void)  //关oled显示 
{
    OLED_WriteData(0x8D,CMD_MODE);
    OLED_WriteData(0x10,CMD_MODE);
    OLED_WriteData(0xAE,CMD_MODE);
} 

//////////////////////////////////////////////////////////////////////////////
void OLED_Display_On(void)//开oled显示
{  
    //电荷泵设置（初始化时必须打开，否则看不到显示）
    OLED_WriteData(0x8D, CMD_MODE);  
    OLED_WriteData(0x14, CMD_MODE);//bit2   0：关闭        1：打开  
    OLED_WriteData(0xAF, CMD_MODE);//0xAF:开显示      
}  

//////////////////////////////////////////////////////////////////////////////
void OLED_Init(void)  //oled参数初始化
{  
    OLED_GPIO_Init();  //端口初始化
    OLED_SCL(1);     
    xDelay_ms(10);  
    OLED_RST(0);  
    xDelay_ms(10);  
    OLED_RST(1);
    OLED_WriteData(0xAE, CMD_MODE);//0xAE:关显示     
    OLED_WriteData(0x00, CMD_MODE);//设置低列地址  
    OLED_WriteData(0x10, CMD_MODE);//设置高列地址  
    //设置行显示的开始地址(0-63)  
    OLED_WriteData(0x40, CMD_MODE);//40-47: (01xxxxx) 
    OLED_WriteData(0x81, CMD_MODE); //设置对比度
    OLED_WriteData(0xff, CMD_MODE);//这个值越大，屏幕越亮(和上条指令一起使用)(0x00-0xff)     
    OLED_WriteData(0xA1, CMD_MODE);//0xA1: 左右反置，  0xA0: 正常显示（默认0xA0）  
    OLED_WriteData(0xC8, CMD_MODE);//0xC8: 上下反置，  0xC0: 正常显示（默认0xC0）  
    //0xA6: 表示正常显示（在面板上1表示点亮，0表示不亮）  
    //0xA7: 表示逆显示（在面板上0表示点亮，1表示不亮）  
    OLED_WriteData(0xA6, CMD_MODE);    
    OLED_WriteData(0xA8, CMD_MODE);//设置多路复用率（1-64）  
    OLED_WriteData(0x3F, CMD_MODE);//（0x01-0x3f）(默认为3f)    
    //设置显示抵消移位映射内存计数器  
    OLED_WriteData(0xD3, CMD_MODE);  
    OLED_WriteData(0x00, CMD_MODE);//（0x00-0x3f）(默认为0x00)  
    //设置显示时钟分频因子/振荡器频率  
    OLED_WriteData(0xD5, CMD_MODE);  
    //低4位定义显示时钟(屏幕的刷新时间)（默认：0000）分频因子= [3:0]+1  
    //高4位定义振荡器频率（默认：1000）  
    OLED_WriteData(0x80, CMD_MODE);//  
    //时钟预充电周期  
    OLED_WriteData(0xD9, CMD_MODE);  
    OLED_WriteData(0xF1, CMD_MODE);//[3:0],PHASE 1;   [7:4] PHASE 2  
    //设置COM硬件应脚配置  
    OLED_WriteData(0xDA, CMD_MODE);  
    OLED_WriteData(0x12, CMD_MODE);//[5:4]  默认：01  
    OLED_WriteData(0xDB, CMD_MODE);//  
    OLED_WriteData(0x40, CMD_MODE);//  
    //设置内存寻址方式  
    OLED_WriteData(0x20, CMD_MODE);  
    //00: 表示水平寻址方式  
    //01: 表示垂直寻址方式  
    //10: 表示页寻址方式（默认方式）  
    OLED_WriteData(0x02, CMD_MODE);//      
    //电荷泵设置（初始化时必须打开，否则看不到显示）  
    OLED_WriteData(0x8D, CMD_MODE);  
    OLED_WriteData(0x14, CMD_MODE);//bit2   0：关闭        1：打开  
    //设置是否全部显示 0xA4: 禁止全部显示  
    OLED_WriteData(0xA4, CMD_MODE);  
    //0xA6: 表示正常显示（在面板上1表示点亮，0表示不亮）  
    //0xA7: 表示逆显示（在面板上0表示点亮，1表示不亮）  
    OLED_WriteData(0xA6, CMD_MODE);//  
    OLED_WriteData(0xAF, CMD_MODE);//0xAF:开显示     
    OLED_WriteData(0xAF, CMD_MODE); //不知道为什么要写两次
    OLED_Clear();
    OLED_SetXY(0,0);
}  

/////////////////////////////////////////////////////////////////////////	
//画一个点
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char Data)
{
	OLED_SetXY(x,y);
	OLED_WriteData(Data,DATA_MODE);
}  

////////////////////////////////////////////////////////////////////////////////////
void Gui_Rectangle(unsigned int x, unsigned int y, unsigned int l, unsigned int w)
{
	static unsigned char dx=0,dy=0;
//	OLED_WriteData(x,DATA_MODE);
	for(dy=y;dy<w;dy++)
	{  	
		OLED_SetXY(dx,dy);
		  for(dx=y;dx<l;dx++)
				 OLED_WriteData(0xFF,DATA_MODE);	
	}		
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void GUI_F6x8(unsigned char x, unsigned char y, unsigned char chr,const unsigned char Font[][6])
{
	unsigned char row,col;
	unsigned char c=0;    
	c = chr - ' ';    //得到偏移后的值
	if(x>Max_Column-1)
	{
		x=0;
		y+=2;
	}	
	for(row=0;row<1;row++)
	{	
	  OLED_SetXY(x,y+row);
		for(col=0;col<6;col++)  //每行6个元素，一个字需要1行字符串
		{		
		  OLED_WriteData(Font[c+row][col],DATA_MODE);
		}
	}		
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void GUI_F8x16(unsigned char x, unsigned char y, unsigned char chr)
{
	unsigned char col;
	unsigned char c=0;    
	c = chr - ' ';    //得到偏移后的值
	if(x>Max_Column-1)
	{
		x=0;
		y+=1;
	} 
	OLED_SetXY(x,y/8);
	for(col=0;col<8;col++)
	OLED_WriteData(F8X16[c][col],DATA_MODE);
	
	OLED_SetXY(x,(y+8)/8);
	for(col=0;col<8;col++)
	OLED_WriteData(F8X16[c][col+8],DATA_MODE);
}

void GUI_F14x14(unsigned char x, unsigned char y, unsigned char c)
{
	unsigned char row,col;
		OLED_SetXY(x,(y+row)/8);
	  for(col=0;col<14;col++)
	  OLED_WriteData(F14x14[c+row][col],DATA_MODE);
		OLED_SetXY(x,(y+row)/8);
	  for(col=0;col<14;col++)
	  OLED_WriteData(F14x14[c+row][col],DATA_MODE);	
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void GUI_F16x16(unsigned char x, unsigned char y,unsigned char chn)
{
	unsigned char row,col;

	for(row=0;row<2;row++)
	{
		OLED_SetXY(x,y+row);
		for(col=0;col<16;col++)  //每行16个元素，一个字需要两行字符串
		{
			OLED_WriteData(Song_F16x16[chn*2+row][col],DATA_MODE);
		} 
  }	
}

////////////////////////////////////////////////////////////////////////////////////
//void GUI_F16x16(unsigned char x, unsigned char y, unsigned char font)  
//{  
//    unsigned char t;
//    
//    OLED_SetXY(x,y);
//    for(t=0;t<16;t++)  //每行16个元素，一个字需要两行字符串
//    {
//			OLED_WriteData(Default_Font16x16[font][t],DATA_MODE);
//    }    
//} 

////////////////////////////////////////////////////////////////////////////////////////////////////
void GUI_F32x32(unsigned char x, unsigned char y, unsigned char font)  
{  
	 unsigned char row,col;
   if(39>=font)
	 {
		 for(row=0;row<4;row++)
		 {
				OLED_SetXY(x,y+row);
				for(col=0;col<16;col++)  //每行16个元素，一个字需要4行字符串
				{
					OLED_WriteData(Seogo_UI32x32[4*font+row][col],DATA_MODE);
				} 
			}	
	 }
	else
	{
		 for(row=0;row<8;row++)
		 {
				OLED_SetXY(x,y+row);
				for(col=0;col<16;col++)  //每行16个元素，一个字需要8行字符串
				{
					OLED_WriteData(Seogo_UI32x32[39*4+8*(font-40)+row][col],DATA_MODE);
				} 
			}			
	}	
} 

////////////////////////////////////////////////////////////////////////////////////////////////////
//显示字符串
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{
		GUI_F8x16(x,y,chr[j]);
		x+= 8 ;
		if(x>120)
		{
			x=0;
			y+=2;  //自动换行写
		}
	 j++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//m^n函数
static unsigned int oled_pow(unsigned char m,unsigned char n)
{
    unsigned int result = 1; 
    while(n--)
		{
			result*=m; 
		}			
    return result;
}

//显示数字
//x,y :起点坐标     
//len :数字的位数
//size:字体大小
//mode:模式    0,填充模式;1,叠加模式
//num:数值(0~4294967295);               
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size)
{             
    unsigned char t = 0, temp = 0;
    unsigned char enshow=0;
    
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                GUI_F6x8(x+(size/2)*t,y,' ',F6x8);
                continue;
            }else enshow=1; 
              
        }
         GUI_F6x8(x+(size/2)*t,y,temp+'0',F6x8); 
    }
} 

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void GUI_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,const unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetXY(x0,y);
    for(x=x0;x<x1;x++)
	  {      
	    OLED_WriteData(BMP[j++],DATA_MODE);
	  }
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LOGO_BMP(const unsigned char BMP[])
{
	 GUI_BMP(0,2,48,8,BMP);//y轴是以8个点写一次
}


