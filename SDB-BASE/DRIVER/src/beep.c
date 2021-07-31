
#include "beep.h"
#include "tim.h"
#include "rrs.h"

Beep_st Beep=
{
 0,//conter
 2,//num
 1,//en
 500//rt
};

//////////////////////////////////////////////////////////////////////////////////////////
void Beep_Thread(Beep_st *Beep_info)
{
	if(0<Beep_info->num)
	{
		if(5*200<=Beep_info->conter++)//�������ʱ��
		{
			Beep_info->conter=0;
			Beep_info->num--;
			Beep_info->rt=500;//����
		}	
	}			
	if(0<Beep_info->rt)
	{
		Beep_info->rt--;
		Beep_info->en=~Beep_info->en;
  }
	else
	{
		Beep_info->en=0;
	}
	BEEP_SW(Beep_info->en);
}

//////////////////////////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
    Beep_Thread(&Beep);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ		
   }	
}

