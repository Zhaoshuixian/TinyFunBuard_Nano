/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related       hardware initialization 
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

#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "common.h"

static unsigned int  timerMsCount;
       unsigned char aRxBuffer;
/** User area the current device state structure*/
dataPoint_t currentDataPoint;


/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
//  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
//  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_POWER:
        currentDataPoint.valuePOWER = dataPointPtr->valuePOWER;
        if(0x01 == currentDataPoint.valuePOWER)
        {
          if(!MWC.Key.Obj.Pwr_SW)
					MWC.Key.Obj.Pwr_SW=1;	
        }
        else
        {
          if(MWC.Key.Obj.Pwr_SW)
					MWC.Key.Obj.Pwr_SW=0;	  
        }
        break;
      case EVENT_FAN_GEAR:
        currentDataPoint.valueFAN_GEAR = dataPointPtr->valueFAN_GEAR;
        if(0x01 == currentDataPoint.valueFAN_GEAR)
        {
					if(MWC.Sensor.OutRoom_PM25.Online)
					{
						
					}
          else
				  {
						 MWC.Fan.Lev=HIGH;
				  }
        }
        else
        {
					if(MWC.Sensor.OutRoom_PM25.Online)
					{
						
					}
          else
				  {
						  MWC.Fan.Lev=LOW; 
				  }
        }
        break;
      case EVENT_RUN_MODE:
        currentDataPoint.valueRUN_MODE = dataPointPtr->valueRUN_MODE;
        if(0x01 == currentDataPoint.valueRUN_MODE)
        {
          if(MWC.Sensor.OutRoom_PM25.Online)
					{
					   MWC.Fan.Sta=Auto;
					}
					{
						 MWC.Fan.Sta=Manul;
					}
        }
        else
        {
           MWC.Fan.Sta=Manul;    
        }
        break;
      case EVENT_NETDATE_UPDATA:
        currentDataPoint.valueNETDATE_UPDATA = dataPointPtr->valueNETDATE_UPDATA;
        if(0x01 == currentDataPoint.valueNETDATE_UPDATA)
        {
           MWC.Sensor.WiFi.NtpReq=1;
        }
        break;
      case EVENT_CLEAR_DO:
        currentDataPoint.valueCLEAR_DO = dataPointPtr->valueCLEAR_DO;
        if(0x01 == currentDataPoint.valueCLEAR_DO)
        {
           MWC.FtrNet.tOn=1;
        }
        if(MWC.FtrNet.tOn)
        {
					 MWC.FtrNet.tOn=0;  
        }
        break;
      case EVENT_ALARM_TYPE:
        currentDataPoint.valueALARM_TYPE = dataPointPtr->valueALARM_TYPE;
        switch(currentDataPoint.valueALARM_TYPE)
        {
          case ALARM_TYPE_VALUE0:
             MWC.System.Alarm.Alarm_SW=CANCEL;
            break;
          case ALARM_TYPE_VALUE1:
             MWC.System.Alarm.Alarm_SW=OPEN;
            break;
          case ALARM_TYPE_VALUE2:
             MWC.System.Alarm.Alarm_SW=CLOSE;
            break;
          case ALARM_TYPE_VALUE3:
             MWC.System.Alarm.Alarm_SW=ALL;
            break;
          default:
						 MWC.System.Alarm.Alarm_SW=CANCEL;
            break;
        }
        break;
      case EVENT_ALARM_OPEN_HOUR:
        currentDataPoint.valueALARM_OPEN_HOUR = dataPointPtr->valueALARM_OPEN_HOUR;
				MWC.Operat.Cont=0;
			   MWC.System.Set.onMode=ALARM_SET_MODE;
			   MWC.LCD.Animat.ExitTime=SET_EXIT_TIME;
			   MWC.System.Alarm.Date.Set_Obj=HOUR;
			   MWC.System.Alarm.Open_Clock.Hour=currentDataPoint.valueALARM_OPEN_HOUR;
        break;
      case EVENT_ALARM_OPEN_MIN:
        currentDataPoint.valueALARM_OPEN_MIN = dataPointPtr->valueALARM_OPEN_MIN;
			  MWC.Operat.Cont=1;
			   MWC.System.Set.onMode=ALARM_SET_MODE;
			   MWC.LCD.Animat.ExitTime=SET_EXIT_TIME;
		  	 MWC.System.Alarm.Date.Set_Obj=MIN;
				 MWC.System.Alarm.Open_Clock.Min=currentDataPoint.valueALARM_OPEN_MIN;			
        break;
      case EVENT_ALARM_CLOSE_HOUR:
        currentDataPoint.valueALARM_CLOSE_HOUR = dataPointPtr->valueALARM_CLOSE_HOUR;
			  MWC.Operat.Cont=2;
			   MWC.System.Set.onMode=ALARM_SET_MODE;
			   MWC.LCD.Animat.ExitTime=SET_EXIT_TIME;
			   MWC.System.Alarm.Date.Set_Obj=HOUR;
			   MWC.System.Alarm.Close_Clock.Hour=currentDataPoint.valueALARM_CLOSE_HOUR;
        break;
      case EVENT_ALARM_CLOSE_MIN:
        currentDataPoint.valueALARM_CLOSE_MIN = dataPointPtr->valueALARM_CLOSE_MIN;
			  MWC.Operat.Cont=3;
			   MWC.System.Set.onMode=ALARM_SET_MODE;
			   MWC.LCD.Animat.ExitTime=SET_EXIT_TIME;
				 MWC.System.Alarm.Date.Set_Obj=MIN;
			   MWC.System.Alarm.Close_Clock.Min=currentDataPoint.valueALARM_CLOSE_MIN;
        break;
      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
				MWC.Sensor.WiFi.Online=0;
			  MWC.Sensor.WiFi.Router=0;//未连入路由点
				MWC.Sensor.WiFi.Config=2;//配置进行
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
				MWC.Sensor.WiFi.Online=0;
				MWC.Sensor.WiFi.Config=1;//配置完成
			  MWC.Sensor.WiFi.Router=1;//接入了路由点
        break;
      case WIFI_DISCON_ROUTER:
				MWC.Sensor.WiFi.Online=0;
			  MWC.Sensor.WiFi.Router=0;//未连入路由点
        break;
      case WIFI_CON_M2M:
         MWC.Sensor.WiFi.Online=1;
        break;
      case WIFI_DISCON_M2M:
				MWC.Sensor.WiFi.Online=0;
        break;
      case WIFI_RSSI:
        break;
      case TRANSPARENT_DATA:
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
				if(MWC.System.Main.Clock.Year!=ptime->year||MWC.System.Main.Clock.Mon!=ptime->month||MWC.System.Main.Clock.Day!=ptime->day||\
				  MWC.System.Main.Clock.Hour!=ptime->hour||MWC.System.Main.Clock.Min!=ptime->minute)
					{
							memcpy(&MWC.System.Main.Clock.Year,&ptime->year,sizeof(MWC.System.Main.Clock.Year));
							memcpy(&MWC.System.Main.Clock.Mon,&ptime->month,sizeof(MWC.System.Main.Clock.Mon));
							memcpy(&MWC.System.Main.Clock.Day,&ptime->day,sizeof(MWC.System.Main.Clock.Day));
							memcpy(&MWC.System.Main.Clock.Hour,&ptime->hour,sizeof(MWC.System.Main.Clock.Hour));
							memcpy(&MWC.System.Main.Clock.Min,&ptime->minute,sizeof(MWC.System.Main.Clock.Min));
							memcpy(&MWC.System.Main.Clock.Sec,&ptime->second,sizeof(MWC.System.Main.Clock.Sec));			
							MWC.System.Main.Clock.Updata=1;
					}	
        break;
      case MODULE_INFO:
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void User_Handle(void)
{
  	currentDataPoint.valuePOWER =MWC.Key.Obj.Pwr_SW;
	  currentDataPoint.valueFAN_GEAR =MWC.Fan.Lev ;
    currentDataPoint.valueRUN_MODE =MWC.Fan.Sta ;
	  currentDataPoint.valueNETDATE_UPDATA =MWC.Sensor.WiFi.NtpReq;
	  currentDataPoint.valueCLEAR_DO = MWC.FtrNet.tOn;
		currentDataPoint.valueALARM_TYPE =MWC. System.Alarm.Alarm_SW;
		currentDataPoint.valueALARM_OPEN_HOUR =MWC.System.Alarm.Open_Clock.Hour ;
		currentDataPoint.valueALARM_OPEN_MIN = MWC.System.Alarm.Open_Clock.Min ;
		currentDataPoint.valueALARM_CLOSE_HOUR =MWC.System.Alarm.Close_Clock.Hour ;
		currentDataPoint.valueALARM_CLOSE_MIN =MWC.System.Alarm.Close_Clock.Min ;
    currentDataPoint.valuePM25_LEV = MWC.Sensor.OutRoom_PM25.Lev_Type;//Add Sensor Data Collection
    currentDataPoint.valueDEVICE_ID =MWC.RS485.Dev_Addr;//Add Sensor Data Collection
    currentDataPoint.valueCLEAR_TIME =MWC.FtrNet.tVal ;//Add Sensor Data Collection
    currentDataPoint.valueRS485_BAUD =MWC.RS485.Baud;//Add Sensor Data Collection
    currentDataPoint.valuePM25 =MWC.Sensor. OutRoom_PM25.Val;//Add Sensor Data Collection
    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void User_Init(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    
//      currentDataPoint.valuePOWER =Obj.Pwr_SW;
//      currentDataPoint.valueFAN_GEAR =Fan.Lev ;
//      currentDataPoint.valueRUN_MODE =Fan.Sta ;
//      currentDataPoint.valueNETDATE_UPDATA = NTP_Req;
//      currentDataPoint.valueCLEAR_DO = FtrNet.tOn;
//      currentDataPoint.valueALARM_TYPE = System.Alarm.Alarm_SW;
//      currentDataPoint.valueALARM_OPEN_HOUR =System.Alarm.Open_Clock.Hour ;
//      currentDataPoint.valueALARM_OPEN_MIN = System.Alarm.Open_Clock.Min ;
//      currentDataPoint.valueALARM_CLOSE_HOUR =System.Alarm.Close_Clock.Hour ;
//      currentDataPoint.valueALARM_CLOSE_MIN =System.Alarm.Close_Clock.Min ;
//      currentDataPoint.valuePM25_LEV = OutRoom_PM25.Lev_Type;
//      currentDataPoint.valueDEVICE_ID = RS485.Dev_Addr;
//      currentDataPoint.valueCLEAR_TIME = FtrNet.tVal;
//      currentDataPoint.valueRS485_BAUD = RS485.Baud;
//      currentDataPoint.valuePM25 = OutRoom_PM25.Val;
    

}


/**
* @brief Millisecond timing maintenance function, milliseconds increment, overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief Read millisecond count

* @param none
* @return millisecond count
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief MCU reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
   __set_FAULTMASK(1);//F1系列
	//__disable_irq();//关闭总中断//F0系列
  NVIC_SystemReset();
}

void TIM3_Init(void)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //Clock enable

    TIM_TimeBaseStructure.TIM_Period = 1000-1;
    TIM_TimeBaseStructure.TIM_Prescaler =48-1; 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 

    TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE );   
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM3, ENABLE); 
}

void TIM3_IRQHandler(void)//TIM3中断入口处理
{
    if (TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET)  
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
        gizTimerMs();
    }
}


/**@} */



/**
* @brief Serial port write operation, send data to WiFi module
*
* @param buf      : buf address
* @param len      : buf length
*
* @return : Return effective data length;-1，return failure
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
	//据协议,非包头部分，如果出现0xFF,对于发送方，则需要在0xFF后添加加0x55，且0xFF后面增加的0x55,既不计入包长度，也不计入校验和的计算。
    for(i=0; i<len; i++)
    {
        USART1_SendData(buf[i]);
        if(i >=2 && buf[i] == 0xFF)  
		USART1_SendData(0x55);//若FF非处于数据的第一、二位，则发0x55
    }
    
    return len;
}  
void USART1_GetData(uint8_t *buf)//接收数据
{
	//uint8_t value = 0;

    if(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET)
    {
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        *buf++ = USART_ReceiveData(USART1);		
		  //len=gizPutData(buf,1);
    }
}

void USART1_IRQHandler(void)//接收WiFi数据缓存
{
    uint8_t value = 0;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET)
    {
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        value = USART_ReceiveData(USART1);
        gizPutData(&value,1);
    }
}

void USART1_SendData(uint8_t data)
{
    USART_SendData(USART1,data);
    while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}
