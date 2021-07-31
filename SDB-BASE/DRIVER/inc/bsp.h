

#ifndef __BSP_H__
#define __BSP_H__

#include "pindef.h"
#include "stm32f10x_conf.h"


typedef struct
{
	unsigned char online;
	int  value;	
}Environ_sensor;

typedef enum
{
	UNCHARGE=0,
  CHARGE,	
}BAT_STA;

typedef enum
{
	PER_10=0,
	PER_20,
	PER_30,
	PER_40,
	PER_50,
	PER_60,
	PER_70,
	PER_80,
	PER_90,
	PER_100,
	
}BAT_PWR;

typedef struct
{
	BAT_STA sta;	
	BAT_PWR pwr;
	
}Battery_obj;

typedef enum
{
	DISCONNECT=0,
  CONNECT,	
  TRANSFER,	
}BT_STA;

typedef struct
{
	BT_STA sta;	
	unsigned char Transfer_Val;
}Bluetooth_obj;

enum
{
  MON,
	TUE,
	WEN,
	THR,
	FRI,
	SAT,
	SUN,
};

typedef enum
{
	UNUSE=0,	
	USING,
}USE_STA;

typedef struct
{
	USE_STA use_flag;
	unsigned char value;	
	
}ProgressBar_obj;

typedef enum
{
	CLOSE=0,
  OPEN,	
}ALRM_STA;

typedef struct
{
	ALRM_STA sta;
}Alarm_obj;

typedef enum
{
	DISCON=0,
  CON,	
}INTERNET_STA;

typedef struct
{
	unsigned char use_flag;
	INTERNET_STA sta;
	
}Internet_obj;

extern Environ_sensor Temperature;

void LED_Init(void);
void OLED_GPIO_Init(void);
void AT24C_GPIO_Init(void);
void FM5807_GPIO_Init(void);
void W5500_GPIO_Init(void);
void W5500_SPI2_Init(void);
void W5500_Init(void);
//void W5500_CS(unsigned char val);
unsigned char SPI2_SendByte(unsigned char byte);
void USART1_Init(void);
void ADC_GPIO_Init(void);
void ADC_DMA_Init(void);
void IR_Init(void);
void NTC_Init(void);
void LED_Thread(void);
#endif


