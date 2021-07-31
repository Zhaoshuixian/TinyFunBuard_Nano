#include "ir.h"
#include "slcd.h"
#include "rrs.h"
#include <string.h>
#include <stdio.h>
ir_st ir;
const unsigned char IR_ID=0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Ir_StatusAnalyze(ir_st *ir_info)
{
	if(ir_info->rec_status&0x80)			//上次有数据被接收到了4.5MS的帧头（ir_info->rec_status^7==1）
	{	
		ir_info->rec_status&=~0X10;                     //(ir_info->rec_status & 1110 1111)	取消上升沿已经被捕获标记（ir_info->rec_status^4=0）
		//**ir_info->rec_status&=~(1<<5);                     //(ir_info->rec_status & 1110 1111)	取消上升沿已经被捕获标记（ir_info->rec_status^4=0）
		if((ir_info->rec_status&0X0F)==0X00)  ir_info->rec_status|=1<<6;//标记已经完成一次按键的键值信息采集（ir_info->rec_status^6=1）
		//** if((ir_info->rec_status&0X0F)==0X00)  ir_info->rec_status|=0x40;//标记已经完成一次按键的键值信息采集（ir_info->rec_status^6=1）
		if((ir_info->rec_status&0X0F)<14)     ir_info->rec_status++;
		else
		{
			ir_info->rec_status&=~(1<<7);		         //清空引导标识（ir_info->rec_status^7=0）
			ir_info->rec_status&=0XF0;			//清空计数器	(ir_info->rec_status & 1111 0000)
			//**ir_info->rec_status&=(F<<4);	       //清空计数器	(ir_info->rec_status & 1111 0000)
		}								 	   	
	}		
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Ir_WaveAnalyze(ir_st *ir_info)
{
	if(IR_DATA())//捕获到上升沿【9ms帧头的捕获】
	{ 
		TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);	//CC4P=1  设置为下降沿捕获
		TIM_SetCounter(TIM1,0);					//清空定时器值
		ir_info->rec_status|=0X10;					//标记上升沿已经被捕获（ir_info->rec_status^4=1）
	}                                                   
	else //若捕捉到下降沿
	{
		ir.rec_dval=TIM_GetCapture1(TIM1);				//读取CCR4也可以清CC4IF标志位
		TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising);      //CC4P=0  设置为上升沿捕获 
		if(ir.rec_dval>4200&&ir.rec_dval<4700) //【4.5ms的帧头捕获】
		{
			ir_info->rec_status|=1<<7;	         //标记成功接收到了引导码
					//**ir_info->rec_status|=0x80;         //标记成功接收到了引导码
			ir.rec_num=0;		//清除按键次数计数器
		}
		else if((ir_info->rec_status&0X10)&&(ir_info->rec_status&0X80))//接收到引导码标志信号电平【9ms + 4.5ms】
		{			
			if(ir.rec_dval>300&&ir.rec_dval<800)	      //560为标准值,560us
			{
				 ir.rec_data<<=1;		     //左移一位.
				 ir.rec_data|=0;	            //接收到0	   
			}
			else if(ir.rec_dval>1400&&ir.rec_dval<1800)   //1680为标准值,1680us
			{
			 ir.rec_data<<=1;		    //左移一位.
			 ir.rec_data|=1;		    //接收到1
			}
			else if(ir.rec_dval>2200&&ir.rec_dval<2600)  //得到按键键值增加的信息 2500为标准值
		 {
			 ir.rec_num++; 	          //按键次数增加1次
			 ir_info->rec_status&=0XF0;	         //清空计时器		
		 }
	 }					 
		ir_info->rec_status&=~(1<<4);      //ir_info->rec_status^4=0
		//**ir_info->rec_status&=~0x10;      //ir_info->rec_status^4=0
	}	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Ir_CodeAnalyze(ir_st *ir_info)
{	
	if(ir_info->rec_status&(1<<6))//得到一个按键的所有信息了（ir_info->rec_status^6==1）
	{ 
		ir_info->data.addr =(ir_info->rec_data>>24)&0xFF;		      //得到地码(Addr)
		ir_info->data.addrn=(ir_info->rec_data>>16)&0xFF;	//得到地址反码(AddrN)
		ir_info->data.cmd =(ir_info->rec_data>>8)&0xFF;           //得到命令码(Cmd)
		ir_info->data.cmdn =ir_info->rec_data&0xFF;             //得到命令反码(CmdN) 				
		if((ir_info->data.addr==(unsigned char)~ir.data.addrn)&&(ir_info->data.addr==IR_ID)&&(ir_info->data.cmd==(unsigned char)~ir_info->data.cmdn))//检验遥控识别码(ID)及地址和命令（纠错） 
		{
			ir_info->valid_data=ir_info->data.cmd;//键值正确		
		}   			
		if((0==ir_info->valid_data)||(0==(ir_info->rec_status&0x80)))//按键数据错误/遥控已经没有按下了
		{
			ir_info->rec_status&=~(1<<6);//清除接收到有效按键标识 ir_info->rec_status^6=0
			ir_info->rec_num=0;	 //清除按键次数计数器
		}
	}	
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void IR_IRQHandler(void)
{ 		    	  
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET) //检测是否发生溢出更新中断事件（即捕获的信号周期大于设定的10MS），判断帧头前9ms
	{
		Ir_StatusAnalyze(&ir);			    
	}
	if(TIM_GetITStatus(TIM1,TIM_IT_CC1)!=RESET) //检测是否发生捕获中断事件
	{	  
		Ir_WaveAnalyze(&ir);	 		     	    					   
	}
  TIM_ClearITPendingBit(TIM1,TIM_IT_Update|TIM_IT_CC1);	 //清除中断事件标志位	    
}



//////////////////////////////////////////////////////////////////////////////////////////////////
void IR_Thread(void)
{   
	Ir_CodeAnalyze(&ir);
//	Ir_UI(&ir);
}


