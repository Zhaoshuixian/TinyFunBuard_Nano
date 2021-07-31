

#include "ntc.h"
#include "string.h"
#include <stdio.h>
#include "slcd.h"
#include "ir.h"

extern volatile  unsigned short  ADC_ConvertedValue; 

Environ_sensor Temperature;

ntc_st  ntc=
{
	{
		//-1	 -2  -3   -4   -5	-6	 -7   -8   -9  -10-----对应温度
		3169,3205,3240,3275,3308,3341,3373,3404,3434,3463,
		//-11 -12 -13 -14 -15 -16 -17 -18 -19 -20-----对应温度
		3492,3519,3545,3570,3590,3620,3643,3664,3686,3706,
		//-21 -22 -23 -24 -25 -26 -27 -28 -29 -30-----对应温度
		//	834,843,849,857,865,873,877,887,893,900,
	},
	{	
		3132,3094,3056,3016,2977,2935,2895,2854,2811,2768,2724,
		2681,2636,2592,2547,2502,2456,2411,2366,2315,2275,
		2229,2182,2136,2093,2048,2003,1958,1914,1870,1827, 
		1782,1742,1701,1658,1613,1577,1536,1497,1458,1421, 
		1383,1347,1309,1277,1244,1207,1170,1143,1111,1079, 
		1051,1021,993,964,938,910,883,861,835, 	
	},//adc_tab int
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Find_Tab(unsigned short int cData,const unsigned short int  pTab[],unsigned char pTab_len)// 形参:当前数据 | 索引表 | 表长度
{
	
//  struct 
//	{
//		unsigned char tab_s; //V-T imageTab start position
//		unsigned char tab_e;  //V-T imageTab endian position 
//		unsigned char tab_c; //V-T image current position
//	}Tab;
	
	unsigned char tab_s=0; //V-T imageTab start position
	unsigned char tab_e;  //V-T imageTab endian position 
	unsigned char tab_c=0; //V-T image current position
  unsigned char k=0;
	tab_e=pTab_len-1;//表末端位置即为表长度值-1
	
	if(pTab[tab_s]<=cData) //如果当前数据小于等于表首数据
	
		return tab_s; //则当前数据即为表首值
	
	else if(pTab[tab_e]>=cData) //如果当前数据大于等于表尾数据
	
		return tab_e;//则当前数据即为表尾值
	
		for(;tab_s<tab_e;)//搜索全表
		{
			tab_c=(tab_s+tab_e)/2; //从表中间查起
			if(pTab[tab_c]==cData) break;
			if(pTab[tab_c]>cData&&cData>pTab[tab_c+1]) break; //若 数据小于表中间值，则以当前位置为表尾查起
			if(cData>pTab[tab_c])  tab_e=tab_c;
			else  tab_s=tab_c;
			if(k++>pTab_len) break;
		}
		if(tab_s>tab_e) return 0;
	return tab_c;//返回  当前的表位置即为所求映射值
}

////////////////////////////////////////////////////////////////////////////////////////////
void Averg_Val(unsigned short int base_val,unsigned char n)
{
	unsigned int sum;
	unsigned char time=0;
	for(;time<n;time++)
	{
	   sum+=base_val;
	}
	sum/=n;
}

////////////////////////////////////////////////////////////////////////////////////////////
void Temperature_Get(Environ_sensor *Temperature_Info)//获得温度数据
{
	Averg_Val(ADC_ConvertedValue,10);
	if(835<=ADC_ConvertedValue&&ADC_ConvertedValue<=3132)
	{
		Temperature_Info->value=Find_Tab(ADC_ConvertedValue,ntc.adc_tabH,60);
	}
	if(3169<=ADC_ConvertedValue&&ADC_ConvertedValue<=3706)
	{
		Temperature_Info->value=Find_Tab(ADC_ConvertedValue,ntc.adc_tabL,20);		
	}
	if(0!=ADC_ConvertedValue)
	{
    Temperature_Info->online=1;		
	}
	else
	{
		Temperature_Info->online=0;	
	}
}


void Temperature_Thread(void)
{
	Temperature_Get(&Temperature);

}
