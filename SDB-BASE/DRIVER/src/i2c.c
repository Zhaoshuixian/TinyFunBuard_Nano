
#include "i2c.h"
#include "delay.h"
#include "bsp.h"

/////////////////////////////for 24CXX////////////////////////////////////////

void AT24_IIC_Init(void)//初始化IIC
{					     
  AT24C_GPIO_Init();
	IIC1_SCL(1);   
	IIC1_SDA(1);

}

/////////////////////////////////////////////////////////////////////

void IIC1_Start(void)//产生IIC起始信号
{
	IIC1_SDA_OUT();     //sda线输出  
	IIC1_SDA(1);	  	  
	IIC1_SCL(1);
  sDelay_us(4);
 	IIC1_SDA(0);//START:when CLK is high,DATA change form high to low 
	sDelay_us(4);
	IIC1_SCL(0);//钳住I2C总线，准备发送或接收数据 
}	 


/////////////////////////////////////////////////////////////////////
void IIC1_Stop(void)//产生IIC停止信号
{
	IIC1_SDA_OUT();//sda线输出
	IIC1_SCL(0);
	IIC1_SDA(0);//STOP:when CLK is high DATA change form low to high
 	sDelay_us(4);
	IIC1_SCL(1); 
	IIC1_SDA(1);//发送I2C总线结束信号
	sDelay_us(4);							   	
}


//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
/////////////////////////////////////////////////////////////////////
unsigned char IIC1_Wait_Ack(void)
{
	unsigned char ucErrTime=0;
	IIC1_SDA_IN();      //SDA设置为输入  
	IIC1_SDA(1);sDelay_us(1);	   
	IIC1_SCL(1);sDelay_us(1);	 
	while(IIC1_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC1_Stop();
			return 1;
		}
	}
	IIC1_SCL(0);//时钟输出0 	   
	return 0;  
} 

/////////////////////////////////////////////////////////////////////
void IIC1_Ack(void)//产生ACK应答
{
	IIC1_SCL(0);
	IIC1_SDA_OUT();
	IIC1_SDA(0);
	sDelay_us(2);
	IIC1_SCL(1);
	sDelay_us(2);
	IIC1_SCL(0);
}

/////////////////////////////////////////////////////////////////////    
void IIC1_NAck(void)//不产生ACK应答		
{
	IIC1_SCL(0);
	IIC1_SDA_OUT();
	IIC1_SDA(1);
	sDelay_us(2);
	IIC1_SCL(1);
	sDelay_us(2);
	IIC1_SCL(0);
}		

/////////////////////////////////////////////////////////////////////    
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答	
void IIC1_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
	  IIC1_SDA_OUT(); 	    
    IIC1_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			IIC1_SDA((txd&0x80)>>7);
			txd<<=1; 	  
			sDelay_us(2);   //对TEA5767这三个延时都是必须的
			IIC1_SCL(1);
			sDelay_us(2); 
			IIC1_SCL(0);	
			sDelay_us(2);
    }	 
} 

/////////////////////////////////////////////////////////////////////  
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
unsigned char IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC1_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
		IIC1_SCL(0); 
		sDelay_us(2);
		IIC1_SCL(1);
		receive<<=1;
		if(IIC1_READ_SDA)receive++;   
		sDelay_us(1); 
    }					 
    if (!ack)
        IIC1_NAck();//发送nACK
    else
        IIC1_Ack(); //发送ACK   
    return receive;
}

//====================================================================================

void FM5807_IIC_Init(void)//初始化IIC
{					     
  FM5807_GPIO_Init();
	IIC2_SCL(1);   
	IIC2_SDA(1);

}

/////////////////////////////////////////////////////////////////////

void IIC2_Start(void)//产生IIC起始信号
{
	IIC2_SDA_OUT();     //sda线输出  
	IIC2_SDA(1);	  	  
	IIC2_SCL(1);
  sDelay_us(30);
 	IIC2_SDA(0);//START:when CLK is high,DATA change form high to low 
	sDelay_us(30);
	IIC2_SCL(0);//钳住I2C总线，准备发送或接收数据 
}	 


/////////////////////////////////////////////////////////////////////
void IIC2_Stop(void)//产生IIC停止信号
{
	IIC2_SDA_OUT();//sda线输出
	IIC2_SCL(0);
	IIC2_SDA(0);//STOP:when CLK is high DATA change form low to high
	sDelay_us(30);
	IIC2_SCL(1); 
	IIC2_SDA(1);//发送I2C总线结束信号
	sDelay_us(30);					   	
}


//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
/////////////////////////////////////////////////////////////////////
unsigned char IIC2_Wait_Ack(void)
{
	unsigned char ucErrTime=0;
	IIC2_SDA_IN();      //SDA设置为输入  
	IIC2_SDA(1);	sDelay_us(30);   
	IIC2_SCL(1);	sDelay_us(30); 
	while(IIC2_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC2_Stop();
			return 1;
		}
	}
	IIC2_SCL(0);//时钟输出0 	   
	return 0;  
} 

/////////////////////////////////////////////////////////////////////
void IIC2_Ack(void)//产生ACK应答
{
	IIC2_SCL(0);
	IIC2_SDA_OUT();
	IIC2_SDA(0);
	sDelay_us(30);
	IIC2_SCL(1);
	sDelay_us(30);
	IIC2_SCL(0);
}

/////////////////////////////////////////////////////////////////////    
void IIC2_NAck(void)//不产生ACK应答		
{
	IIC2_SCL(0);
	IIC2_SDA_OUT();
	IIC2_SDA(1);
	sDelay_us(30);
	IIC2_SCL(1);
	sDelay_us(30);
	IIC2_SCL(0);
}		

/////////////////////////////////////////////////////////////////////    
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答	
void IIC2_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
	  IIC2_SDA_OUT(); 	    
    IIC2_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			IIC2_SDA((txd&0x80)>>7);
			txd<<=1; 	  
	    sDelay_us(10);
			IIC2_SCL(1);
	    sDelay_us(10);
			IIC2_SCL(0);	
	    sDelay_us(30);
    }	 
} 

/////////////////////////////////////////////////////////////////////  
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
unsigned char IIC2_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC2_SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
	{
		IIC2_SCL(0); 
		sDelay_us(30);
		IIC2_SCL(1);
		receive<<=1;
		if(IIC2_READ_SDA)
		{
			receive++; 
		}			
		sDelay_us(30); 
  }					 
	(!ack)?(IIC2_NAck()):(IIC2_Ack());  
    return receive;
}

//=========================================================================
/////////////////////////////for BMP280////////////////////////////////////////
/*
void BMP280_IIC_Init(void)//初始化IIC
{					     
  BMP280_GPIO_Init();
	IIC3_SCL(1);   
	IIC3_SDA(1);

}

/////////////////////////////////////////////////////////////////////

void IIC3_Start(void)//产生IIC起始信号
{
	IIC3_SDA_OUT();     //sda线输出  
	IIC3_SDA(1);	  	  
	IIC3_SCL(1);
  sDelay_us(4);
 	IIC3_SDA(0);//START:when CLK is high,DATA change form high to low 
	sDelay_us(4);
	IIC3_SCL(0);//钳住I2C总线，准备发送或接收数据 
}	 


/////////////////////////////////////////////////////////////////////
void IIC3_Stop(void)//产生IIC停止信号
{
	IIC3_SDA_OUT();//sda线输出
	IIC3_SCL(0);
	IIC3_SDA(0);//STOP:when CLK is high DATA change form low to high
 	sDelay_us(4);
	IIC3_SCL(1); 
	IIC3_SDA(1);//发送I2C总线结束信号
	sDelay_us(4);							   	
}


//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
/////////////////////////////////////////////////////////////////////
unsigned char IIC3_Wait_Ack(void)
{
	unsigned char ucErrTime=0;
	IIC3_SDA_IN();      //SDA设置为输入  
	IIC3_SDA(1);sDelay_us(1);	   
	IIC3_SCL(1);sDelay_us(1);	 
	while(IIC3_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC3_Stop();
			return 1;
		}
	}
	IIC3_SCL(0);//时钟输出0 	   
	return 0;  
} 

/////////////////////////////////////////////////////////////////////
void IIC3_Ack(void)//产生ACK应答
{
	IIC3_SCL(0);
	IIC3_SDA_OUT();
	IIC3_SDA(0);
	sDelay_us(2);
	IIC3_SCL(1);
	sDelay_us(2);
	IIC3_SCL(0);
}

/////////////////////////////////////////////////////////////////////    
void IIC3_NAck(void)//不产生ACK应答		
{
	IIC3_SCL(0);
	IIC3_SDA_OUT();
	IIC3_SDA(1);
	sDelay_us(2);
	IIC3_SCL(1);
	sDelay_us(2);
	IIC3_SCL(0);
}		

/////////////////////////////////////////////////////////////////////    
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答	
void IIC3_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
	  IIC3_SDA_OUT(); 	    
    IIC3_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			IIC3_SDA((txd&0x80)>>7);
			txd<<=1; 	  
			sDelay_us(2);   //对TEA5767这三个延时都是必须的
			IIC3_SCL(1);
			sDelay_us(2); 
			IIC3_SCL(0);	
			sDelay_us(2);
    }	 
} 

/////////////////////////////////////////////////////////////////////  
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
unsigned char IIC3_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC3_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
		IIC3_SCL(0); 
		sDelay_us(2);
		IIC3_SCL(1);
		receive<<=1;
		if(IIC3_READ_SDA)receive++;   
		sDelay_us(1); 
    }					 
    if (!ack)
        IIC3_NAck();//发送nACK
    else
        IIC3_Ack(); //发送ACK   
    return receive;
}
*/























