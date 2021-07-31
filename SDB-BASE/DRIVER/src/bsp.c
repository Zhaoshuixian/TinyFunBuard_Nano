

#include "bsp.h"
#include "lcd.h"
#include "delay.h"
#include "lcd_gui.h"
#include "beep.h"
#include "at24cxx.h"
#include "stdio.h"
#include "tim.h"
#include "string.h"

#define ADC1_DR_Address    ((unsigned int)0x4001244C)
volatile  unsigned short   ADC_ConvertedValue; 


void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;   
	RCC_APB2PeriphClockCmd(RLED_PCC,ENABLE);
	RCC_APB2PeriphClockCmd(BLED_PCC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  RLED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(RLED_PORT, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin =  BLED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(BLED_PORT, &GPIO_InitStructure); 	
}


////////////////////////////////////////////////////////////////////////////////
#ifdef  USE_096OLED
void OLED_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;   
	RCC_APB2PeriphClockCmd(OLED_PCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin =  OLED_DC_PIN|OLED_RST_PIN|OLED_SCL_PIN|OLED_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OLED_PORT, &GPIO_InitStructure);
	OLED_SCL(1);
  OLED_SDA(1);
	OLED_RST(1);
	OLED_DC(1);
}

#else
////////////////////////////////////////////////////////////////////////////////
void OLED_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;   
	RCC_APB2PeriphClockCmd(OLED_PCC_A,ENABLE);
	RCC_APB2PeriphClockCmd(OLED_PCC_B,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  OLED_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OLED_PORT_A, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin =  OLED_CS_PIN|OLED_A0_PIN|OLED_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OLED_PORT_B, &GPIO_InitStructure);  	
}
#endif
////////////////////////////////////////////////////////////////////////////////
void AT24C_GPIO_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure; 
 	RCC_APB2PeriphClockCmd(AT24CXX_PCC,ENABLE); 	
	
	GPIO_InitStructure.GPIO_Pin = AT24CXX_SCL_PIN|AT24CXX_SDA_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AT24CXX_PORT, &GPIO_InitStructure);  	
}

////////////////////////////////////////////////////////////////////////////////
void DHT11_GPIO_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure; 
 	RCC_APB2PeriphClockCmd(DHT_PCC,ENABLE); 	
	
	GPIO_InitStructure.GPIO_Pin = DHT_DATA_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT_PORT, &GPIO_InitStructure);  	
}

////////////////////////////////////////////////////////////////////////////////
void FM5807_GPIO_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(FM5807_PCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = FM5807_SCL_PIN|FM5807_SDA_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FM5807_PORT, &GPIO_InitStructure);  
}

////////////////////////////////////////////////////////////////////////////////
void W5500_GPIO_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure; 
	EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(W5500_PCC_A,ENABLE);
	RCC_APB2PeriphClockCmd(W5500_PCC_B,ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = W5500_MISO_PIN|W5500_MOSI_PIN|W5500_SCLK_PIN ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(W5500_PORT_B, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = W5500_SCS_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(W5500_PORT_B, &GPIO_InitStructure);
  GPIO_SetBits(W5500_PORT_B,W5500_SCS_PIN); 

  GPIO_InitStructure.GPIO_Pin = W5500_RESET_PIN ; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(W5500_PORT_A, &GPIO_InitStructure);
  GPIO_SetBits(W5500_PORT_A, W5500_RESET_PIN);
	
  GPIO_InitStructure.GPIO_Pin = W5500_INT_PIN ; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(W5500_PORT_A, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
  EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_ResetBits(W5500_PORT_B,W5500_RESET_PIN);
 
}

////////////////////////////////////////////////////////////////////////////////
void W5500_SPI2_Init(void)
{
	SPI_InitTypeDef   SPI_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);

}

////////////////////////////////////////////////////////////////////////////////
void W5500_Init(void)
{
	W5500_GPIO_Init();
	W5500_SPI2_Init();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void ADC_GPIO_Init(void) //设置ADC采样PA0端口信号
{ 
	GPIO_InitTypeDef GPIO_InitStructure;    
  RCC_APB2PeriphClockCmd(NTC_PCC, ENABLE);	
  
  GPIO_InitStructure.GPIO_Pin = NTC_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO设置为模拟输入
  GPIO_Init(NTC_PORT,&GPIO_InitStructure);   
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
 void ADC_DMA_Init(void)/*配置ADC1的工作模式为MDA模式  */
{
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); //使能MDA1时钟
  DMA_DeInit(DMA1_Channel1);  //指定DMA通道
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//设置DMA外设地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)&ADC_ConvertedValue;	//设置DMA内存地址，ADC转换结果直接放入该地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为设置为数据传输的来源
  DMA_InitStructure.DMA_BufferSize = 1;	//DMA缓冲区设置为1；
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  DMA_Cmd(DMA1_Channel1, ENABLE);  //使能DMA通道
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//使能ADC1时钟
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //使用独立模式，扫描模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //无需外接触发器
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //使用数据右对齐
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;  // 只有1个转换通道
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5); //通道1采样周期55.5个时钟周期
  ADC_DMACmd(ADC1,ENABLE);	 //使能ADC的DMA
  ADC_Cmd(ADC1,ENABLE); //使能ADC1
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //开始转换
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void NTC_Init(void)
{
	ADC_GPIO_Init();
	ADC_DMA_Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void IR_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(IR_PCC, ENABLE);// 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStruct.GPIO_Pin = IR_PIN ;   
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //

	GPIO_Init(IR_PORT,&GPIO_InitStruct); //
}

  
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void IR_Init(void)
{
	IR_GPIO_Init();
}



















