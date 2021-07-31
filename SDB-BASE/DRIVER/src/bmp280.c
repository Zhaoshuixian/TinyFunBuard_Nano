

#include "bmp280.h"
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"


unsigned short dig_t1, dig_p1;
signed short dig_t2, dig_t3, dig_p2, dig_p3, dig_p4;
signed short dig_p5, dig_p6, dig_p7, dig_p8, dig_p9;

float fine=0;
 
Environ_sensor Pressure={1,0.0};
 
unsigned char BMP280_ReadOneByte(unsigned short int ReadAddr)
{				  
	unsigned char temp=0;		  	    																 
  IIC3_Start();  
  IIC3_Send_Byte(BMP280_ADDR_WR);   //发送器件地址0XA0,写数据 	
	IIC3_Wait_Ack(); 
  IIC3_Send_Byte(ReadAddr%256);   //发送低地址
	IIC3_Wait_Ack();	    
	IIC3_Start();  	 	   
	IIC3_Send_Byte(BMP280_ADDR_RD);           //进入接收模式			   
	IIC3_Wait_Ack();	 
  temp=IIC3_Read_Byte(0);		   
  IIC3_Stop();//产生一个停止条件	    
	return temp;
}

void BMP280_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
  IIC2_Start();  
	IIC2_Send_Byte(BMP280_ADDR_WR);   //发送器件地址PCF8563_Write_Add,写数据 
	IIC2_Wait_Ack();	   
  IIC2_Send_Byte(WriteAddr%256);   //发送低地址
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //发送字节							   
	IIC2_Wait_Ack();  		    	   
  IIC2_Stop();//产生一个停止条件 
  sDelay_ms(5);	 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void BMP280_Init(void)
{
	BMP280_IIC_Init(); 
	if(0x58 == BMP280_ReadOneByte(0xd0))
	{
		dig_t1 = (BMP280_ReadOneByte(BME280_DIG_T1_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_T1_LSB_REG);
		dig_t2 = (BMP280_ReadOneByte(BME280_DIG_T2_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_T2_LSB_REG);
		dig_t3 = (BMP280_ReadOneByte(BME280_DIG_T3_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_T3_LSB_REG);
		dig_p1 = (BMP280_ReadOneByte(BME280_DIG_P1_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_P1_LSB_REG);
		dig_p2 = (BMP280_ReadOneByte(BME280_DIG_P2_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_P2_LSB_REG);
		dig_p3 = (BMP280_ReadOneByte(BME280_DIG_P3_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_P3_LSB_REG);
		dig_p4 = (BMP280_ReadOneByte(BME280_DIG_P4_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_P4_LSB_REG);
		dig_p5 = (BMP280_ReadOneByte(BME280_DIG_P5_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_P5_LSB_REG);
		dig_p6 = (BMP280_ReadOneByte(BME280_DIG_P6_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_P6_LSB_REG);
		dig_p7 = (BMP280_ReadOneByte(BME280_DIG_P7_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_P7_LSB_REG);
		dig_p8 = (BMP280_ReadOneByte(BME280_DIG_P8_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_P8_LSB_REG);
		dig_p9 = (BMP280_ReadOneByte(BME280_DIG_P9_MSB_REG)<<8)|BMP280_ReadOneByte(BME280_DIG_P9_LSB_REG);
	}
  BMP280_WriteOneByte(BME280_RST_REG,BMP280_RESET_VALUE);
	xDelay_ms(50);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float bmp280_compensate_pressure_double(float adc_pres)
{
		float  pressure,var1, var2;
		var1 = (fine / 2.0) - 64000.0;
		var2 = var1 * var1*(dig_p6) / 32768.0;
		var2 = var2 + var1 * (dig_p5)*2.0;
		var2 = (var2 / 4.0) + ((dig_p4)*65536.0);
		var1 = (dig_p3)*var1*var1 / 524288.0 + (dig_p2)*var1/524288.0;
		var1 = (1.0 + var1 / 32768.0)*(dig_p1);
	  if(0.0==var1) return 0;
		pressure = 1048576.0 - adc_pres;
		pressure = (pressure - (var2 / 4096.0))*6250.0 / var1;
		var1 = (dig_p9)*pressure*pressure / 2147483648.0;
		var2 = pressure * (dig_p8) / 32768.0;
		pressure = pressure + (var1 + var2 + (dig_p7)) / 16.0;	
	
	  return  pressure;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float bmp280_compensate_temperature_double(float adc_temp)
{
		float temperature, var1, var2;
		var1 = (adc_temp / 16384.0 - dig_t1 / 1024.0)*(dig_t2);
		var2 = ((adc_temp / 131072.0 - dig_t1 /8192.0)*(adc_temp / 131072.0 - dig_t1 / 8192.0))*dig_t3;
		fine = var1 + var2;
		temperature = fine/5120.0;	
   	
	  return  temperature;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float BMP280_GetTemperature(void)
{
	  int adc_temp;
	  float temperature;
	  unsigned short msb, lsb, xlsb;	
		msb = BMP280_ReadOneByte(BME280_TEMPERATURE_MSB_REG);
		lsb = BMP280_ReadOneByte(BME280_TEMPERATURE_LSB_REG);
		xlsb= BMP280_ReadOneByte(BME280_TEMPERATURE_XLSB_REG);
	
		adc_temp = (msb<<16|lsb<<8|xlsb)>>4;	
	  temperature=bmp280_compensate_temperature_double(adc_temp);
	
		return temperature;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float BMP280_GetPressure(void)
{
	  int adc_pres;
	  float pressure;
    unsigned short msb, lsb, xlsb;	
		msb = BMP280_ReadOneByte(BME280_PRESSURE_MSB_REG);
		lsb = BMP280_ReadOneByte(BME280_PRESSURE_LSB_REG);
		xlsb= BMP280_ReadOneByte(BME280_PRESSURE_XLSB_REG);
		adc_pres= (msb<<16|lsb<<8|xlsb)>>4;
	 pressure=bmp280_compensate_pressure_double(adc_pres);
	
	 return pressure;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void BMP280_Thread(void)
{
	Pressure.online=1;
	Pressure.value=BMP280_GetPressure();
}
