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
	if(ir_info->rec_status&0x80)			//�ϴ������ݱ����յ���4.5MS��֡ͷ��ir_info->rec_status^7==1��
	{	
		ir_info->rec_status&=~0X10;                     //(ir_info->rec_status & 1110 1111)	ȡ���������Ѿ��������ǣ�ir_info->rec_status^4=0��
		//**ir_info->rec_status&=~(1<<5);                     //(ir_info->rec_status & 1110 1111)	ȡ���������Ѿ��������ǣ�ir_info->rec_status^4=0��
		if((ir_info->rec_status&0X0F)==0X00)  ir_info->rec_status|=1<<6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ���ir_info->rec_status^6=1��
		//** if((ir_info->rec_status&0X0F)==0X00)  ir_info->rec_status|=0x40;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ���ir_info->rec_status^6=1��
		if((ir_info->rec_status&0X0F)<14)     ir_info->rec_status++;
		else
		{
			ir_info->rec_status&=~(1<<7);		         //���������ʶ��ir_info->rec_status^7=0��
			ir_info->rec_status&=0XF0;			//��ռ�����	(ir_info->rec_status & 1111 0000)
			//**ir_info->rec_status&=(F<<4);	       //��ռ�����	(ir_info->rec_status & 1111 0000)
		}								 	   	
	}		
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Ir_WaveAnalyze(ir_st *ir_info)
{
	if(IR_DATA())//���������ء�9ms֡ͷ�Ĳ���
	{ 
		TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);	//CC4P=1  ����Ϊ�½��ز���
		TIM_SetCounter(TIM1,0);					//��ն�ʱ��ֵ
		ir_info->rec_status|=0X10;					//����������Ѿ�������ir_info->rec_status^4=1��
	}                                                   
	else //����׽���½���
	{
		ir.rec_dval=TIM_GetCapture1(TIM1);				//��ȡCCR4Ҳ������CC4IF��־λ
		TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising);      //CC4P=0  ����Ϊ�����ز��� 
		if(ir.rec_dval>4200&&ir.rec_dval<4700) //��4.5ms��֡ͷ����
		{
			ir_info->rec_status|=1<<7;	         //��ǳɹ����յ���������
					//**ir_info->rec_status|=0x80;         //��ǳɹ����յ���������
			ir.rec_num=0;		//�����������������
		}
		else if((ir_info->rec_status&0X10)&&(ir_info->rec_status&0X80))//���յ��������־�źŵ�ƽ��9ms + 4.5ms��
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
			 ir_info->rec_status&=0XF0;	         //��ռ�ʱ��		
		 }
	 }					 
		ir_info->rec_status&=~(1<<4);      //ir_info->rec_status^4=0
		//**ir_info->rec_status&=~0x10;      //ir_info->rec_status^4=0
	}	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Ir_CodeAnalyze(ir_st *ir_info)
{	
	if(ir_info->rec_status&(1<<6))//�õ�һ��������������Ϣ�ˣ�ir_info->rec_status^6==1��
	{ 
		ir_info->data.addr =(ir_info->rec_data>>24)&0xFF;		      //�õ�����(Addr)
		ir_info->data.addrn=(ir_info->rec_data>>16)&0xFF;	//�õ���ַ����(AddrN)
		ir_info->data.cmd =(ir_info->rec_data>>8)&0xFF;           //�õ�������(Cmd)
		ir_info->data.cmdn =ir_info->rec_data&0xFF;             //�õ������(CmdN) 				
		if((ir_info->data.addr==(unsigned char)~ir.data.addrn)&&(ir_info->data.addr==IR_ID)&&(ir_info->data.cmd==(unsigned char)~ir_info->data.cmdn))//����ң��ʶ����(ID)����ַ��������� 
		{
			ir_info->valid_data=ir_info->data.cmd;//��ֵ��ȷ		
		}   			
		if((0==ir_info->valid_data)||(0==(ir_info->rec_status&0x80)))//�������ݴ���/ң���Ѿ�û�а�����
		{
			ir_info->rec_status&=~(1<<6);//������յ���Ч������ʶ ir_info->rec_status^6=0
			ir_info->rec_num=0;	 //�����������������
		}
	}	
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void IR_IRQHandler(void)
{ 		    	  
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET) //����Ƿ�����������ж��¼�����������ź����ڴ����趨��10MS�����ж�֡ͷǰ9ms
	{
		Ir_StatusAnalyze(&ir);			    
	}
	if(TIM_GetITStatus(TIM1,TIM_IT_CC1)!=RESET) //����Ƿ��������ж��¼�
	{	  
		Ir_WaveAnalyze(&ir);	 		     	    					   
	}
  TIM_ClearITPendingBit(TIM1,TIM_IT_Update|TIM_IT_CC1);	 //����ж��¼���־λ	    
}



//////////////////////////////////////////////////////////////////////////////////////////////////
void IR_Thread(void)
{   
	Ir_CodeAnalyze(&ir);
//	Ir_UI(&ir);
}


