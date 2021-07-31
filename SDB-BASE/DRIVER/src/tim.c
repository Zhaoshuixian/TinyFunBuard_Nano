
#include "tim.h"
#include "beep.h"

//APB1=36MHz   APB2=72MHz
void TIM1_Init(void)
{

		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		 
		NVIC_InitTypeDef NVIC_InitStructure;
	 TIM_ICInitTypeDef  TIM_ICInitStructure;  
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

		TIM_TimeBaseStructure.TIM_Period = 10000;                   //设定计数器自动重装值 最大10ms溢出;
		TIM_TimeBaseStructure.TIM_Prescaler =72-1;                  //
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;             //每次向上溢出都产生更新事件 ,此参数很重要，使用定时器1，15,16,17时必须初始化，其它定时器可以不用初始化
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);               //根据指定的参数初始化TIMx
	                                     //使能定时器1	
    //输入捕获比较模式配置  
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  //TIM1_CH1
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//下降沿捕获
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
		TIM_ICInitStructure.TIM_ICFilter = 0x03;
		TIM_ICInit(TIM1, &TIM_ICInitStructure);//初始化定时器输入捕获通道
	
    TIM_ClearFlag(TIM1,TIM_FLAG_Update);
		TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);          //允许更新和捕获中断 	
		TIM_Cmd(TIM1,ENABLE ); 	//使能定时器3	
	
		NVIC_InitStructure.NVIC_IRQChannel =  TIM1_UP_IRQn|TIM1_CC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
		NVIC_Init(&NVIC_InitStructure);   
}

void TIM2_Init(void)
{

		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		 
		NVIC_InitTypeDef NVIC_InitStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
		TIM_TimeBaseStructure.TIM_Period = 200; //设定计数器自动重装值 最大1ms溢出;
		TIM_TimeBaseStructure.TIM_Prescaler =36-1; //
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); //根据指定的参数初始化TIMx
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//允许更新中断 
		TIM_Cmd(TIM2,ENABLE); 	//使能定时器2
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
		NVIC_Init(&NVIC_InitStructure);   
}


void TIM3_Init(void)
{

		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		 
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//使能定时器3时钟
	
		TIM_TimeBaseStructure.TIM_Period = 1000; //设定计数器自动重装值 最大1ms溢出;
		TIM_TimeBaseStructure.TIM_Prescaler =72-1; //
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); //根据指定的参数初始化TIMx
    TIM_ClearFlag(TIM3,TIM_FLAG_Update);
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//允许更新中断 
		TIM_Cmd(TIM3,ENABLE); 	//使能定时器3
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
		NVIC_Init(&NVIC_InitStructure);   
}

