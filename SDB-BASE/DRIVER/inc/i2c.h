
#ifndef __I2C_H__
#define __I2C_H__

#include "pindef.h"
#include "stm32f10x_conf.h"

//for AT24Cxx
//IO�������� /*BITBAND OPERATION*/

#define IIC1_SDA_IN()  {AT24CXX_PORT->CRH&=0xFFFFF0FF;AT24CXX_PORT->CRH|=((unsigned int)8<<8);}//PB10-SDA//PB11-SCL
#define IIC1_SDA_OUT() {AT24CXX_PORT->CRH&=0xFFFFF0FF;AT24CXX_PORT->CRH|=(unsigned int)(3<<8);}

#define IIC1_SCL(x)    (x==0?GPIO_ResetBits(AT24CXX_PORT,AT24CXX_SCL_PIN):GPIO_SetBits(AT24CXX_PORT,AT24CXX_SCL_PIN)) //SCL
#define IIC1_SDA(x)    (x==0?GPIO_ResetBits(AT24CXX_PORT,AT24CXX_SDA_PIN):GPIO_SetBits(AT24CXX_PORT,AT24CXX_SDA_PIN))  //SDA	 
#define IIC1_READ_SDA  GPIO_ReadInputDataBit(AT24CXX_PORT,AT24CXX_SDA_PIN)

//for FM5807

#define IIC2_SDA_IN()  {FM5807_PORT->CRH &=0x0FFFF0FF;FM5807_PORT->CRH|=((unsigned int)8<<8);}//PA7-SDA
#define IIC2_SDA_OUT() {FM5807_PORT->CRH &=0x0FFFF0FF;FM5807_PORT->CRH|=(unsigned int)(3<<8);}

#define IIC2_SCL(x)    (x==0?GPIO_ResetBits(FM5807_PORT,FM5807_SCL_PIN):GPIO_SetBits(FM5807_PORT,FM5807_SCL_PIN)) //SCL
#define IIC2_SDA(x)    (x==0?GPIO_ResetBits(FM5807_PORT,FM5807_SDA_PIN):GPIO_SetBits(FM5807_PORT,FM5807_SDA_PIN))  //SDA	 
#define IIC2_READ_SDA  GPIO_ReadInputDataBit(FM5807_PORT,FM5807_SDA_PIN)


////for BMP280

//#define IIC3_SDA_IN()  {BMP280_PORT->CRH &=0x0FFFFF0F;BMP280_PORT->CRH|=((unsigned int)8<<4);}//PA4--SDA 
//#define IIC3_SDA_OUT() {BMP280_PORT->CRH &=0x0FFFFF0F;BMP280_PORT->CRH|=(unsigned int)(3<<4);}

//#define IIC3_SCL(x)    (x==0?GPIO_ResetBits(BMP280_PORT,BMP280_SCL_PIN):GPIO_SetBits(BMP280_PORT,BMP280_SCL_PIN)) //SCL
//#define IIC3_SDA(x)    (x==0?GPIO_ResetBits(BMP280_PORT,BMP280_SDA_PIN):GPIO_SetBits(BMP280_PORT,BMP280_SDA_PIN))  //SDA	 
//#define IIC3_READ_SDA  GPIO_ReadInputDataBit(BMP280_PORT,BMP280_SDA_PIN)

//IIC���в�������
////////////////////////////////////////////////////////////////////////////////////
void AT24_IIC_Init(void);                //��ʼ��IIC��IO��	
void IIC1_Start(void);				//����IIC��ʼ�ź�
void IIC1_Stop(void);	  			//����IICֹͣ�ź�
void IIC1_Send_Byte(unsigned char txd);			//IIC����һ���ֽ�
unsigned char IIC1_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char IIC1_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC1_Ack(void);					//IIC����ACK�ź�
void IIC1_NAck(void);				//IIC������ACK�ź�

////////////////////////////////////////////////////////////////////////////////////
void FM5807_IIC_Init(void);                //��ʼ��IIC��IO��	
void IIC2_Start(void);				//����IIC��ʼ�ź�
void IIC2_Stop(void);	  			//����IICֹͣ�ź�
void IIC2_Send_Byte(unsigned char txd);			//IIC����һ���ֽ�
unsigned char IIC2_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char IIC2_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC2_Ack(void);					//IIC����ACK�ź�
void IIC2_NAck(void);				//IIC������ACK�ź�

////////////////////////////////////////////////////////////////////////////////////
void BMP280_IIC_Init(void);                //��ʼ��IIC��IO��	
void IIC3_Start(void);				//����IIC��ʼ�ź�
void IIC3_Stop(void);	  			//����IICֹͣ�ź�
void IIC3_Send_Byte(unsigned char txd);			//IIC����һ���ֽ�
unsigned char IIC3_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char IIC3_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC3_Ack(void);					//IIC����ACK�ź�
void IIC3_NAck(void);				//IIC������ACK�ź�
#endif
