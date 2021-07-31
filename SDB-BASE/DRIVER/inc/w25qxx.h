

#ifndef __FLASH_H
#define __FLASH_H			    

#include "stm32f10x.h"
	  
//W25Qϵ��оƬ��ȡIDֵ   
#define W25Q08 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

#define FLASH_SIZE  128*1024*1024   //FLASH ��СΪ16M�ֽ�

#define spi_num  1                //�궨�� W25QXX_Init(u8 SPIx)�����ʼ���������������������ʵ���õ��ĸ�SPI�ھ�����Ϊ��ӦSPI����ż���
								 //�������õ�SPI1����spi_num����Ϊ1�ǿ�

extern u16 W25QXX_TYPE;		    //����W25QXXоƬ�ͺ�	


				 
////////////////////////////////////////////////////////////////////////////
 
//ָ���
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


u16  W25QXX_ReadID(void);  	    		                                           //��ȡFLASH ID
void W25QXX_Init(u8 num);
u8   W25QXX_ReadSR(u8 num);        		                                        //��ȡ״̬�Ĵ��� 
void W25QXX_Write_SR(u8 sr,u8 num);                                            //д״̬�Ĵ���
void W25QXX_Write_Enable(u8 num);  		                                      //дʹ�� 
void W25QXX_Write_Disable(u8 num);		                                     //д����
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead,u8 num) ;      //��ȡflash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);          //д��flash
void W25QXX_Erase_Chip(u8 num);    	  	                                 //��Ƭ����
void W25QXX_Erase_Sector(u32 Dst_Addr,u8 num);	                        //��������
void W25QXX_Wait_Busy(void);           	                               //�ȴ�����
void W25QXX_PowerDown(u8 num);        	                              //�������ģʽ
void W25QXX_WAKEUP(u8 num);				                             //����
#endif
















