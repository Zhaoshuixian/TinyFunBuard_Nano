

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
extern void AT24XFactory(void); //FLASH恢复出厂默认值
extern void AT24XRestart(void); //FLASH系统参数初始化
void AT24Cxx_AutoStore(void); //FLASH自动存储
unsigned char AT24CXX_ReadOneByte(unsigned short int ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(unsigned short int WriteAddr,unsigned int DataToWrite,unsigned char Len);//指定地址开始写入指定长度的数据
unsigned int AT24CXX_ReadLenByte(unsigned short int ReadAddr,unsigned char Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(unsigned short int WriteAddr,unsigned char *pBuffer,unsigned short int NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(unsigned short int ReadAddr,unsigned char *pBuffer,unsigned short int NumToRead);   	//从指定地址开始读出指定长度的数据
void AT24XRestart1(void);
extern void AT24CXXCheck(void);
unsigned char AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC

#endif


