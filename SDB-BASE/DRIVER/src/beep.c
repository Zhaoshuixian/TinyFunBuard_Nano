
#include "beep.h"
#include "tim.h"
#include "rrs.h"

buzzer_st buzzer;

//////////////////////////////////////////////////////////////////////////////////////////
void buzzer_driver(buzzer_st *const me)
{
	if(0<me->num)
	{
		if(5*200<=me->conter++)//�������ʱ��
		{
			me->conter=0;
			me->num--;
			me->rt=500;//����
		}	
	}			
	if(0<me->rt)
	{
		me->rt--;
		me->en=~me->en;
  }
	else
	{
		me->en=0;
	}
	BEEP_SW(me->en);
}

//////////////////////////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
        buzzer_driver(&buzzer);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ		
   }	
}

