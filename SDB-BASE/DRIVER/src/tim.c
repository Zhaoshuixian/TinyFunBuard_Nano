
#include "tim.h"
#include "beep.h"

//APB1=36MHz   APB2=72MHz
void tim1_init(void)
{

		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		 
		NVIC_InitTypeDef NVIC_InitStructure;
	 TIM_ICInitTypeDef  TIM_ICInitStructure;  
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

		TIM_TimeBaseStructure.TIM_Period = 10000;                   //�趨�������Զ���װֵ ���10ms���;
		TIM_TimeBaseStructure.TIM_Prescaler =72-1;                  //
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;             //ÿ��������������������¼� ,�˲�������Ҫ��ʹ�ö�ʱ��1��15,16,17ʱ�����ʼ����������ʱ�����Բ��ó�ʼ��
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);               //����ָ���Ĳ�����ʼ��TIMx
	                                     //ʹ�ܶ�ʱ��1	
    //���벶��Ƚ�ģʽ����  
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  //TIM1_CH1
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�½��ز���
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
		TIM_ICInitStructure.TIM_ICFilter = 0x03;
		TIM_ICInit(TIM1, &TIM_ICInitStructure);//��ʼ����ʱ�����벶��ͨ��
	
    TIM_ClearFlag(TIM1,TIM_FLAG_Update);
		TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);          //�������ºͲ����ж� 	
		TIM_Cmd(TIM1,ENABLE ); 	//ʹ�ܶ�ʱ��3	
	
		NVIC_InitStructure.NVIC_IRQChannel =  TIM1_UP_IRQn|TIM1_CC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
		NVIC_Init(&NVIC_InitStructure);   
}

void tim2_init(void)
{

		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		 
		NVIC_InitTypeDef NVIC_InitStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
		TIM_TimeBaseStructure.TIM_Period = 200; //�趨�������Զ���װֵ ���1ms���;
		TIM_TimeBaseStructure.TIM_Prescaler =36-1; //
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//���������ж� 
		TIM_Cmd(TIM2,ENABLE); 	//ʹ�ܶ�ʱ��2
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
		NVIC_Init(&NVIC_InitStructure);   
}


void tim3_init(void)
{

		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		 
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
	
		TIM_TimeBaseStructure.TIM_Period = 1000; //�趨�������Զ���װֵ ���1ms���;
		TIM_TimeBaseStructure.TIM_Prescaler =72-1; //
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx
    TIM_ClearFlag(TIM3,TIM_FLAG_Update);
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//���������ж� 
		TIM_Cmd(TIM3,ENABLE); 	//ʹ�ܶ�ʱ��3
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
		NVIC_Init(&NVIC_InitStructure);   
}

