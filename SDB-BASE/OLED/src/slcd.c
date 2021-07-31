

#include "slcd.h"
#include "font.h"
#include "bsp.h"

#define SPI_MODE
/*
SPIģʽ�£�RST��Ҫ�У�������������ʾ
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
    OLED_WriteData((((x&0xf0)>>4)|0x10),CMD_MODE);//��4λ  
    OLED_WriteData((x & 0x0f)|0x01,CMD_MODE);//��4λ  
}

//////////////////////////////////////////////////////////////////////////////
//������һ��ʼ����д���ˣ���д����д����д���ݣ�������������ȷ��������Ļ���кܶ���㣬˵��û�������ɹ���
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
void OLED_Display_Off(void)  //��oled��ʾ 
{
    OLED_WriteData(0x8D,CMD_MODE);
    OLED_WriteData(0x10,CMD_MODE);
    OLED_WriteData(0xAE,CMD_MODE);
} 

//////////////////////////////////////////////////////////////////////////////
void OLED_Display_On(void)//��oled��ʾ
{  
    //��ɱ����ã���ʼ��ʱ����򿪣����򿴲�����ʾ��
    OLED_WriteData(0x8D, CMD_MODE);  
    OLED_WriteData(0x14, CMD_MODE);//bit2   0���ر�        1����  
    OLED_WriteData(0xAF, CMD_MODE);//0xAF:����ʾ      
}  

//////////////////////////////////////////////////////////////////////////////
void OLED_Init(void)  //oled������ʼ��
{  
    OLED_GPIO_Init();  //�˿ڳ�ʼ��
    OLED_SCL(1);     
    xDelay_ms(10);  
    OLED_RST(0);  
    xDelay_ms(10);  
    OLED_RST(1);
    OLED_WriteData(0xAE, CMD_MODE);//0xAE:����ʾ     
    OLED_WriteData(0x00, CMD_MODE);//���õ��е�ַ  
    OLED_WriteData(0x10, CMD_MODE);//���ø��е�ַ  
    //��������ʾ�Ŀ�ʼ��ַ(0-63)  
    OLED_WriteData(0x40, CMD_MODE);//40-47: (01xxxxx) 
    OLED_WriteData(0x81, CMD_MODE); //���öԱȶ�
    OLED_WriteData(0xff, CMD_MODE);//���ֵԽ����ĻԽ��(������ָ��һ��ʹ��)(0x00-0xff)     
    OLED_WriteData(0xA1, CMD_MODE);//0xA1: ���ҷ��ã�  0xA0: ������ʾ��Ĭ��0xA0��  
    OLED_WriteData(0xC8, CMD_MODE);//0xC8: ���·��ã�  0xC0: ������ʾ��Ĭ��0xC0��  
    //0xA6: ��ʾ������ʾ���������1��ʾ������0��ʾ������  
    //0xA7: ��ʾ����ʾ���������0��ʾ������1��ʾ������  
    OLED_WriteData(0xA6, CMD_MODE);    
    OLED_WriteData(0xA8, CMD_MODE);//���ö�·�����ʣ�1-64��  
    OLED_WriteData(0x3F, CMD_MODE);//��0x01-0x3f��(Ĭ��Ϊ3f)    
    //������ʾ������λӳ���ڴ������  
    OLED_WriteData(0xD3, CMD_MODE);  
    OLED_WriteData(0x00, CMD_MODE);//��0x00-0x3f��(Ĭ��Ϊ0x00)  
    //������ʾʱ�ӷ�Ƶ����/����Ƶ��  
    OLED_WriteData(0xD5, CMD_MODE);  
    //��4λ������ʾʱ��(��Ļ��ˢ��ʱ��)��Ĭ�ϣ�0000����Ƶ����= [3:0]+1  
    //��4λ��������Ƶ�ʣ�Ĭ�ϣ�1000��  
    OLED_WriteData(0x80, CMD_MODE);//  
    //ʱ��Ԥ�������  
    OLED_WriteData(0xD9, CMD_MODE);  
    OLED_WriteData(0xF1, CMD_MODE);//[3:0],PHASE 1;   [7:4] PHASE 2  
    //����COMӲ��Ӧ������  
    OLED_WriteData(0xDA, CMD_MODE);  
    OLED_WriteData(0x12, CMD_MODE);//[5:4]  Ĭ�ϣ�01  
    OLED_WriteData(0xDB, CMD_MODE);//  
    OLED_WriteData(0x40, CMD_MODE);//  
    //�����ڴ�Ѱַ��ʽ  
    OLED_WriteData(0x20, CMD_MODE);  
    //00: ��ʾˮƽѰַ��ʽ  
    //01: ��ʾ��ֱѰַ��ʽ  
    //10: ��ʾҳѰַ��ʽ��Ĭ�Ϸ�ʽ��  
    OLED_WriteData(0x02, CMD_MODE);//      
    //��ɱ����ã���ʼ��ʱ����򿪣����򿴲�����ʾ��  
    OLED_WriteData(0x8D, CMD_MODE);  
    OLED_WriteData(0x14, CMD_MODE);//bit2   0���ر�        1����  
    //�����Ƿ�ȫ����ʾ 0xA4: ��ֹȫ����ʾ  
    OLED_WriteData(0xA4, CMD_MODE);  
    //0xA6: ��ʾ������ʾ���������1��ʾ������0��ʾ������  
    //0xA7: ��ʾ����ʾ���������0��ʾ������1��ʾ������  
    OLED_WriteData(0xA6, CMD_MODE);//  
    OLED_WriteData(0xAF, CMD_MODE);//0xAF:����ʾ     
    OLED_WriteData(0xAF, CMD_MODE); //��֪��ΪʲôҪд����
    OLED_Clear();
    OLED_SetXY(0,0);
}  

/////////////////////////////////////////////////////////////////////////	
//��һ����
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
	c = chr - ' ';    //�õ�ƫ�ƺ��ֵ
	if(x>Max_Column-1)
	{
		x=0;
		y+=2;
	}	
	for(row=0;row<1;row++)
	{	
	  OLED_SetXY(x,y+row);
		for(col=0;col<6;col++)  //ÿ��6��Ԫ�أ�һ������Ҫ1���ַ���
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
	c = chr - ' ';    //�õ�ƫ�ƺ��ֵ
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
		for(col=0;col<16;col++)  //ÿ��16��Ԫ�أ�һ������Ҫ�����ַ���
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
//    for(t=0;t<16;t++)  //ÿ��16��Ԫ�أ�һ������Ҫ�����ַ���
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
				for(col=0;col<16;col++)  //ÿ��16��Ԫ�أ�һ������Ҫ4���ַ���
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
				for(col=0;col<16;col++)  //ÿ��16��Ԫ�أ�һ������Ҫ8���ַ���
				{
					OLED_WriteData(Seogo_UI32x32[39*4+8*(font-40)+row][col],DATA_MODE);
				} 
			}			
	}	
} 

////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʾ�ַ���
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
			y+=2;  //�Զ�����д
		}
	 j++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//m^n����
static unsigned int oled_pow(unsigned char m,unsigned char n)
{
    unsigned int result = 1; 
    while(n--)
		{
			result*=m; 
		}			
    return result;
}

//��ʾ����
//x,y :�������     
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ    0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);               
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

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
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
	 GUI_BMP(0,2,48,8,BMP);//y������8����дһ��
}


