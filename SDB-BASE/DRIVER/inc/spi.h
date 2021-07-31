/*********************************************************************
 *
 *                           spi.c
 *
 *********************************************************************
 * 描    述: 单片机TFT_FSMC驱动函数头文件
 * 开发平台: MDK5
 * 公    司: 			
 * 网    址: 
 * 作者			日期			      注释			    备注
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 飞云创意电子		18/07/27		原始文件         SPI驱动函数头文件 
 ********************************************************************/

#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"

 
void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节 	

void SPI2_Init(void);			 //初始化SPI2口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI2速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
		 
#endif

