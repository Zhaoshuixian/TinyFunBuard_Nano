#include "ir.h"
#include "slcd.h"
#include "rrs.h"
#include <string.h>
#include <stdio.h>

infrade_st iinfrader;
const unsigned char IR_ID=0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Ir_StatusAnalyze(infrade_st *const me)
{
	if(me->rec_status&0x80)			//�ϴ������ݱ����յ���4.5MS��֡ͷ��me->rec_status^7==1��
	{	
		me->rec_status&=~0X10;                     //(me->rec_status & 1110 1111)	ȡ���������Ѿ��������ǣ�me->rec_status^4=0��
		//**me->rec_status&=~(1<<5);                     //(me->rec_status & 1110 1111)	ȡ���������Ѿ��������ǣ�me->rec_status^4=0��
		if((me->rec_status&0X0F)==0X00)  me->rec_status|=1<<6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ���me->rec_status^6=1��
		//** if((me->rec_status&0X0F)==0X00)  me->rec_status|=0x40;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ���me->rec_status^6=1��
		if((me->rec_status&0X0F)<14)     me->rec_status++;
		else
		{
			me->rec_status&=~(1<<7);		         //���������ʶ��me->rec_status^7=0��
			me->rec_status&=0XF0;			//��ռ�����	(me->rec_status & 1111 0000)
			//**me->rec_status&=(F<<4);	       //��ռ�����	(me->rec_status & 1111 0000)
		}								 	   	
	}		
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Ir_WaveAnalyze(infrade_st *const me)
{
	if(IR_DATA())//���������ء�9ms֡ͷ�Ĳ���
	{ 
		TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);	//CC4P=1  ����Ϊ�½��ز���
		TIM_SetCounter(TIM1,0);					//��ն�ʱ��ֵ
		me->rec_status|=0X10;					//����������Ѿ�������me->rec_status^4=1��
	}                                                   
	else //����׽���½���
	{
		ir.rec_dval=TIM_GetCapture1(TIM1);				//��ȡCCR4Ҳ������CC4IF��־λ
		TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising);      //CC4P=0  ����Ϊ�����ز��� 
		if(ir.rec_dval>4200&&ir.rec_dval<4700) //��4.5ms��֡ͷ����
		{
			me->rec_status|=1<<7;	         //��ǳɹ����յ���������
					//**me->rec_status|=0x80;         //��ǳɹ����յ���������
			ir.rec_num=0;		//�����������������
		}
		else if((me->rec_status&0X10)&&(me->rec_status&0X80))//���յ��������־�źŵ�ƽ��9ms + 4.5ms��
		{			
			if(ir.rec_dval>300&&ir.rec_dval<800)	      //560Ϊ��׼ֵ,560us
			{
				 ir.rec_data<<=1;		     //����һλ.
				 ir.rec_data|=0;	            //���յ�0	   
			}
			else if(ir.rec_dval>1400&&ir.rec_dval<1800)   //1680Ϊ��׼ֵ,1680us
			{
			 ir.rec_data<<=1;		    //����һλ.
			 ir.rec_data|=1;		    //���յ�1
			}
			else if(ir.rec_dval>2200&&ir.rec_dval<2600)  //�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ
		 {
			 ir.rec_num++; 	          //������������1��
			 me->rec_status&=0XF0;	         //��ռ�ʱ��		
		 }
	 }					 
		me->rec_status&=~(1<<4);      //me->rec_status^4=0
		//**me->rec_status&=~0x10;      //me->rec_status^4=0
	}	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Ir_CodeAnalyze(infrade_st *const me)
{	
	if(me->rec_status&(1<<6))//�õ�һ��������������Ϣ�ˣ�me->rec_status^6==1��
	{ 
		me->data.addr =(me->rec_data>>24)&0xFF;		      //�õ�����(Addr)
		me->data.addrn=(me->rec_data>>16)&0xFF;	//�õ���ַ����(AddrN)
		me->data.cmd =(me->rec_data>>8)&0xFF;           //�õ�������(Cmd)
		me->data.cmdn =me->rec_data&0xFF;             //�õ������(CmdN) 				
		if((me->data.addr==(unsigned char)~ir.data.addrn)&&(me->data.addr==IR_ID)&&(me->data.cmd==(unsigned char)~me->data.cmdn))//����ң��ʶ����(ID)����ַ����������� 
		{
			me->valid_data=me->data.cmd;//��ֵ��ȷ		
		}   			
		if((0==me->valid_data)||(0==(me->rec_status&0x80)))//�������ݴ���/ң���Ѿ�û�а�����
		{
			me->rec_status&=~(1<<6);//������յ���Ч������ʶ me->rec_status^6=0
			me->rec_num=0;	 //�����������������
		}
	}	
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void IR_IRQHandler(void)
{ 		    	  
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET) //
	{
		Ir_StatusAnalyze(&ir);			    
	}
	if(TIM_GetITStatus(TIM1,TIM_IT_CC1)!=RESET) //
	{	  
		Ir_WaveAnalyze(&ir);	 		     	    					   
	}
  TIM_ClearITPendingBit(TIM1,TIM_IT_Update|TIM_IT_CC1);	 //	    
}



