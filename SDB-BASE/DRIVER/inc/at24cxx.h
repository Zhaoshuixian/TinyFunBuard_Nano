

#ifndef __AT24CXX_H__
#define __AT24CXX_H__


#include "i2c.h"

#define AT24C01		127       /**/
#define AT24C02		255      /*0XFF*/
#define AT24C04		511      /**/
#define AT24C08		1023     /**/
#define AT24C16		2047     /**/
#define AT24C32		4095     /**/
#define AT24C64	  8191     /**/
#define AT24C128	16383   /**/
#define AT24C256	32767   /**/

#define EE_TYPE  AT24C02

void DefaultConfig(void);
extern void AT24XFactory(void); //FLASH�ָ�����Ĭ��ֵ
extern void AT24XRestart(void); //FLASHϵͳ������ʼ��
void AT24Cxx_AutoStore(void); //FLASH�Զ��洢
unsigned char AT24CXX_ReadOneByte(unsigned short int ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(unsigned short int WriteAddr,unsigned int DataToWrite,unsigned char Len);//ָ����ַ��ʼд��ָ�����ȵ�����
unsigned int AT24CXX_ReadLenByte(unsigned short int ReadAddr,unsigned char Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(unsigned short int WriteAddr,unsigned char *pBuffer,unsigned short int NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(unsigned short int ReadAddr,unsigned char *pBuffer,unsigned short int NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����
void AT24XRestart1(void);
extern void AT24CXXCheck(void);
unsigned char AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC

#endif


