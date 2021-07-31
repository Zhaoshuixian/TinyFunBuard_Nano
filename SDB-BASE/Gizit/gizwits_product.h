/**
************************************************************
* @file         gizwits_product.h
* @brief        Corresponding gizwits_product.c header file (including product hardware and software version definition)
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#ifndef _GIZWITS_PRODUCT_H
#define _GIZWITS_PRODUCT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f10x.h"
#include "gizwits_protocol.h"

/**
* MCU software version
*/
#define SOFTWARE_VERSION "03030000"
/**
* MCU hardware version
*/
#define HARDWARE_VERSION "03010100"


/**
* Communication module model
*/
#define MODULE_TYPE 0 //0,WIFI ;1,GPRS


/**@name TIM3 related macro definition
* @{
*/
#define TIMER 					TIM3
#define TIMER_IRQ 				TIM3_IRQn
#define TIMER_RCC 				RCC_APB1Periph_TIM3
#define TIMER_IRQ_FUN 			TIM3_IRQHandler
/**@} */

/**@name USART related macro definition
* @{
*/



/**@} */

/** User area the current device state structure*/
extern dataPoint_t currentDataPoint;

void TIM3_Init(void);
void USART1_GPIO_Config(void);
void USART1_PARA_Config(void);
void USART1_NVIC_Config(void);
void USART1_Init(void);
void UserData_Capture(void);//采集数据
void USART1_SendData(uint8_t data);//发送数据
void USART1_GetData(uint8_t *buf);//接收数据
void User_Init(void);
void User_Handle(void);
void mcuRestart(void);
uint32_t gizGetTimerCount(void);
int32_t uartWrite(uint8_t *buf, uint32_t len);
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
