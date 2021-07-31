
#include "i2c.h"
#include "delay.h"
#include "bsp.h"

/////////////////////////////for 24CXX////////////////////////////////////////

void AT24_IIC_Init(void)//��ʼ��IIC
{					     
  AT24C_GPIO_Init();
	IIC1_SCL(1);   
	IIC1_SDA(1);

}

/////////////////////////////////////////////////////////////////////

void IIC1_Start(void)//����IIC��ʼ�ź�
{
	IIC1_SDA_OUT();     //sda�����  
	IIC1_SDA(1);	  	  
	IIC1_SCL(1);
  sDelay_us(4);
 	IIC1_SDA(0);//START:when CLK is high,DATA change form high to low 
	sDelay_us(4);
	IIC1_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	 


/////////////////////////////////////////////////////////////////////
void IIC1_Stop(void)//����IICֹͣ�ź�
{
	IIC1_SDA_OUT();//sda�����
	IIC1_SCL(0);
	IIC1_SDA(0);//STOP:when CLK is high DATA change form low to high
 	sDelay_us(4);
	IIC1_SCL(1); 
	IIC1_SDA(1);//����I2C���߽����ź�
	sDelay_us(4);							   	
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
/////////////////////////////////////////////////////////////////////
unsigned char IIC1_Wait_Ack(void)
{
	unsigned char ucErrTime=0;
	IIC1_SDA_IN();      //SDA����Ϊ����  
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
	IIC1_SCL(0);//ʱ�����0 	   
	return 0;  
} 

/////////////////////////////////////////////////////////////////////
void IIC1_Ack(void)//����ACKӦ��
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
void IIC1_NAck(void)//������ACKӦ��		
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��	
void IIC1_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
	  IIC1_SDA_OUT(); 	    
    IIC1_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
			IIC1_SDA((txd&0x80)>>7);
			txd<<=1; 	  
			sDelay_us(2);   //��TEA5767��������ʱ���Ǳ����
			IIC1_SCL(1);
			sDelay_us(2); 
			IIC1_SCL(0);	
			sDelay_us(2);
    }	 
} 

/////////////////////////////////////////////////////////////////////  
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
unsigned char IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC1_SDA_IN();//SDA����Ϊ����
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
        IIC1_NAck();//����nACK
    else
        IIC1_Ack(); //����ACK   
    return receive;
}

//====================================================================================

void FM5807_IIC_Init(void)//��ʼ��IIC
{					     
  FM5807_GPIO_Init();
	IIC2_SCL(1);   
	IIC2_SDA(1);

}

/////////////////////////////////////////////////////////////////////

void IIC2_Start(void)//����IIC��ʼ�ź�
{
	IIC2_SDA_OUT();     //sda�����  
	IIC2_SDA(1);	  	  
	IIC2_SCL(1);
  sDelay_us(30);
 	IIC2_SDA(0);//START:when CLK is high,DATA change form high to low 
	sDelay_us(30);
	IIC2_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	 


/////////////////////////////////////////////////////////////////////
void IIC2_Stop(void)//����IICֹͣ�ź�
{
	IIC2_SDA_OUT();//sda�����
	IIC2_SCL(0);
	IIC2_SDA(0);//STOP:when CLK is high DATA change form low to high
	sDelay_us(30);
	IIC2_SCL(1); 
	IIC2_SDA(1);//����I2C���߽����ź�
	sDelay_us(30);					   	
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
/////////////////////////////////////////////////////////////////////
unsigned char IIC2_Wait_Ack(void)
{
	unsigned char ucErrTime=0;
	IIC2_SDA_IN();      //SDA����Ϊ����  
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
	IIC2_SCL(0);//ʱ�����0 	   
	return 0;  
} 

/////////////////////////////////////////////////////////////////////
void IIC2_Ack(void)//����ACKӦ��
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
void IIC2_NAck(void)//������ACKӦ��		
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��	
void IIC2_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
	  IIC2_SDA_OUT(); 	    
    IIC2_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
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
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
unsigned char IIC2_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC2_SDA_IN();//SDA����Ϊ����
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
void BMP280_IIC_Init(void)//��ʼ��IIC
{					     
  BMP280_GPIO_Init();
	IIC3_SCL(1);   
	IIC3_SDA(1);

}

/////////////////////////////////////////////////////////////////////

void IIC3_Start(void)//����IIC��ʼ�ź�
{
	IIC3_SDA_OUT();     //sda�����  
	IIC3_SDA(1);	  	  
	IIC3_SCL(1);
  sDelay_us(4);
 	IIC3_SDA(0);//START:when CLK is high,DATA change form high to low 
	sDelay_us(4);
	IIC3_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	 


/////////////////////////////////////////////////////////////////////
void IIC3_Stop(void)//����IICֹͣ�ź�
{
	IIC3_SDA_OUT();//sda�����
	IIC3_SCL(0);
	IIC3_SDA(0);//STOP:when CLK is high DATA change form low to high
 	sDelay_us(4);
	IIC3_SCL(1); 
	IIC3_SDA(1);//����I2C���߽����ź�
	sDelay_us(4);							   	
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
/////////////////////////////////////////////////////////////////////
unsigned char IIC3_Wait_Ack(void)
{
	unsigned char ucErrTime=0;
	IIC3_SDA_IN();      //SDA����Ϊ����  
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
	IIC3_SCL(0);//ʱ�����0 	   
	return 0;  
} 

/////////////////////////////////////////////////////////////////////
void IIC3_Ack(void)//����ACKӦ��
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
void IIC3_NAck(void)//������ACKӦ��		
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��	
void IIC3_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
	  IIC3_SDA_OUT(); 	    
    IIC3_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
			IIC3_SDA((txd&0x80)>>7);
			txd<<=1; 	  
			sDelay_us(2);   //��TEA5767��������ʱ���Ǳ����
			IIC3_SCL(1);
			sDelay_us(2); 
			IIC3_SCL(0);	
			sDelay_us(2);
    }	 
} 

/////////////////////////////////////////////////////////////////////  
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
unsigned char IIC3_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC3_SDA_IN();//SDA����Ϊ����
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
        IIC3_NAck();//����nACK
    else
        IIC3_Ack(); //����ACK   
    return receive;
}
*/























