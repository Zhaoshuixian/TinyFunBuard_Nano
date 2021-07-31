/*********************************************************************
 *
 *                           w25qxx.h
 *
 *********************************************************************
 * 描    述: 单片机外部FALSH-W25Q128驱动函数头文件 
 * 开发平台: MDK5
 ********************************************************************/

#ifndef __FLASH_H
#define __FLASH_H			    

#include "stm32f10x.h"
	  
//W25Q系列芯片读取ID值   
#define W25Q08 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

#define FLASH_SIZE  128*1024*1024   //FLASH 大小为16M字节

#define spi_num  1                //宏定义 W25QXX_Init(u8 SPIx)这个初始化函数的输入参数，这里实际用到哪个SPI口就设置为对应SPI口序号即可
								 //如这里用到SPI1，则将spi_num定义为1那可

extern u16 W25QXX_TYPE;		    //定义W25QXX芯片型号	


				 
////////////////////////////////////////////////////////////////////////////
 
//指令表
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


u16  W25QXX_ReadID(void);  	    		                                           //读取FLASH ID
void W25QXX_Init(u8 num);
u8   W25QXX_ReadSR(u8 num);        		                                        //读取状态寄存器 
void W25QXX_Write_SR(u8 sr,u8 num);                                            //写状态寄存器
void W25QXX_Write_Enable(u8 num);  		                                      //写使能 
void W25QXX_Write_Disable(u8 num);		                                     //写保护
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead,u8 num) ;      //读取flash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);          //写入flash
void W25QXX_Erase_Chip(u8 num);    	  	                                 //整片擦除
void W25QXX_Erase_Sector(u32 Dst_Addr,u8 num);	                        //扇区擦除
void W25QXX_Wait_Busy(void);           	                               //等待空闲
void W25QXX_PowerDown(u8 num);        	                              //进入掉电模式
void W25QXX_WAKEUP(u8 num);				                             //唤醒
#endif
















