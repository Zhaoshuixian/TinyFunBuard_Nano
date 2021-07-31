
#ifndef   __PINDEF_H__
#define   __PINDEF_H__

#include "stm32f10x.h"

#ifdef USE_096OLED
//////////////////////096_OLED_PORT/////////////////////////////////
#define OLED_PCC             RCC_APB2Periph_GPIOA
#define OLED_PORT            GPIOA
#define OLED_SCL_PIN         GPIO_Pin_5
#define OLED_SDA_PIN         GPIO_Pin_4
#define OLED_RST_PIN         GPIO_Pin_3
#define OLED_DC_PIN          GPIO_Pin_2

#define OLED_SCL(x)           (0==x)?(GPIO_ResetBits(OLED_PORT,OLED_SCL_PIN)):(GPIO_SetBits(OLED_PORT,OLED_SCL_PIN))
#define OLED_SDA(x)           (0==x)?(GPIO_ResetBits(OLED_PORT,OLED_SDA_PIN)):(GPIO_SetBits(OLED_PORT,OLED_SDA_PIN))
#define OLED_RST(x)           (0==x)?(GPIO_ResetBits(OLED_PORT,OLED_RST_PIN)):(GPIO_SetBits(OLED_PORT,OLED_RST_PIN))
#define OLED_DC(x)            (0==x)?(GPIO_ResetBits(OLED_PORT,OLED_DC_PIN)):(GPIO_SetBits(OLED_PORT,OLED_DC_PIN))
#else
//////////////////////144_OLED_PORT/////////////////////////////////
#define OLED_PCC_A             RCC_APB2Periph_GPIOA
#define OLED_PORT_A            GPIOA
#define OLED_SCL_PIN           GPIO_Pin_1

#define OLED_PCC_B             RCC_APB2Periph_GPIOB
#define OLED_PORT_B            GPIOB
#define OLED_CS_PIN            GPIO_Pin_7  //PB7
#define OLED_SDA_PIN           GPIO_Pin_1  //PB1
#define OLED_RST_PIN           GPIO_Pin_5  //PB5
#define OLED_A0_PIN            GPIO_Pin_0  //PB0
#define OLED_BLK_PIN           GPIO_Pin_10  //PB10
#endif
//////////////////////FM5807_PORT/////////////////////////////////
#define FM5807_PCC               RCC_APB2Periph_GPIOB
#define FM5807_PORT              GPIOB
#define FM5807_SCL_PIN           GPIO_Pin_9
#define FM5807_SDA_PIN           GPIO_Pin_8

//////////////////////USER_USART_PORT/////////////////////////////////
#define USER_USART_NUM            USART1
#define USER_USART_PCC            RCC_APB2Periph_GPIOA
#define USER_USART_PORT           GPIOA
#define USER_USART_TX_PIN         GPIO_Pin_9
#define USER_USART_RX_PIN         GPIO_Pin_10
#define USER_USART_ISR            USART1_IRQHandler
//////////////////////BEEP_PORT/////////////////////////////////
#define BEEP_PCC                  RCC_APB2Periph_GPIOC
#define BEEP_PORT                 GPIOC
#define BEEP_PIN                  GPIO_Pin_13
#define BEEP_SW(x)                (x)?(GPIO_ResetBits(BEEP_PORT,BEEP_PIN)):(GPIO_SetBits(BEEP_PORT,BEEP_PIN))
//////////////////////W5500_PORT/////////////////////////////////
#define W5500_PCC_A               RCC_APB2Periph_GPIOA
#define W5500_PORT_A              GPIOA
#define W5500_RESET_PIN           GPIO_Pin_7
#define W5500_INT_PIN             GPIO_Pin_8

#define W5500_PCC_B               RCC_APB2Periph_GPIOB
#define W5500_PORT_B              GPIOB
#define W5500_SCS_PIN             GPIO_Pin_12
#define W5500_SCLK_PIN            GPIO_Pin_13
#define W5500_MISO_PIN            GPIO_Pin_14
#define W5500_MOSI_PIN            GPIO_Pin_15

#define W5500_SCS(x)              (0==x)?(GPIO_ResetBits(W5500_PORT_B,W5500_SCS_PIN)):(GPIO_SetBits(W5500_PORT_B,W5500_SCS_PIN)) 
#define W5500_RESET()             {GPIO_ResetBits(W5500_PORT_B,W5500_RESET_PIN),xDelay_us(5),GPIO_SetBits(W5500_PORT_B,W5500_RESET_PIN),xDelay_ms(500);}
#define W5500_IRQHandler          EXTI9_5_IRQHandler


//////////////////////AT24C_PORT/////////////////////////////////
#define AT24CXX_PCC               RCC_APB2Periph_GPIOA
#define AT24CXX_PORT              GPIOA
#define AT24CXX_SCL_PIN           GPIO_Pin_6
#define AT24CXX_SDA_PIN           GPIO_Pin_7

//////////////////////LED_PORT/////////////////////////////////
#define RLED_PCC                RCC_APB2Periph_GPIOA
#define RLED_PORT               GPIOA
#define RLED_PIN                GPIO_Pin_15
#define BLED_PCC                RCC_APB2Periph_GPIOB
#define BLED_PORT               GPIOB
#define BLED_PIN                GPIO_Pin_11

#define RLED_SW(x)              (x)?(GPIO_ResetBits(RLED_PORT,RLED_PIN)):(GPIO_SetBits(RLED_PORT,RLED_PIN)) 
#define BLED_SW(x)              (x)?(GPIO_ResetBits(BLED_PORT,BLED_PIN)):(GPIO_SetBits(BLED_PORT,BLED_PIN)) 
//////////////////////IR_PORT/////////////////////////////////
#define IR_PCC                RCC_APB2Periph_GPIOA  
#define IR_PORT               GPIOA
#define IR_PIN                GPIO_Pin_8 //TIM1_CH1
#define IR_DATA()               GPIO_ReadInputDataBit(IR_PORT,IR_PIN)
//////////////////////NTC_PORT/////////////////////////////////
#define NTC_PCC               RCC_APB2Periph_GPIOA
#define NTC_PORT              GPIOA
#define NTC_PIN               GPIO_Pin_0
//PA0==ADC-IN0

//////////////////////ESP8266_PORT/////////////////////////////////
#define ESP_PCC                RCC_APB2Periph_GPIOA  
#define ESP_PORT               GPIOA
#define ESP_RXD_PIN            GPIO_Pin_9 //USART1
#define ESP_TXD_PIN            GPIO_Pin_10 //USART1
#define ESP_PWR_PIN            GPIO_Pin_10 //USART1

//////////////////////DHT11_PORT/////////////////////////////////
#define DHT_PCC                RCC_APB2Periph_GPIOB
#define DHT_PORT               GPIOB
#define DHT_DATA_PIN           GPIO_Pin_3

//////////////////////USB_PORT/////////////////////////////////
#define USB_PCC                RCC_APB2Periph_GPIOA
#define USB_PORT               GPIOA
#define USB_DM_PIN             GPIO_Pin_11
#define USB_DP_PIN             GPIO_Pin_12

#define USB_EN_PCC             RCC_APB2Periph_GPIOF
#define USB_EN_PORT            GPIOF
#define USB_EN_PIN             GPIO_Pin_6


#endif

