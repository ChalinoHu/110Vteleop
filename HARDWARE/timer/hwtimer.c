
#include "stm32f10x_tim.h"
#include "hwtimer.h"
#include "lcd.h"
#include "adc.h"
#include "touch.h"

int count = 0;
int value,valuelt;

int value0[40],value1[3];

u8 flag1 = 1;	//首次打开高电平标志位

int num = 0;  //存储电压下标

u8 flag = 0;  ///状态标志位

u8 daotong1=0,daotong2=0,daoda1s=0;   //三个时间标志位

u8 daotongdaotong = 0; //最终导通标志位

u8 showf = 1;		//显示标志位

u8 kaishi = 0,kaishi1 = 0;



// 25ms采集AD
void TIM3_Int_Init(void)
{
 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 99; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx				//根据合适情况打开或者关闭定时器	 
}


void TIM4_Int_Init(void)
{
 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 499; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx				//根据合适情况打开或者关闭定时器	 
}



//10ms采一次AD
void  TIM3_IRQHandler(void)   //TIM3中断
{
			                   
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  	//检查TIM3更新中断发生与否
		{
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); 	 //清除TIMx更新中断标志 		 
			
			value = ADC_con();
		
			if(value>-70&&value<70) flag = 1;  //故障
			
			if(value<-70)
			{
				flag = 0;	//断开
				valuelt = value;
			}
			
		if(value > 70)
		{
			if(flag1)
			{
				flag = 1;
				flag1 = 0;
			}
			
		}
		
		if(valuelt<-70&&value>70)	
		{
			kaishi = 1;
		}
		
		
		
		if(kaishi)
		{
			value0[num++] = value;
			
			if(num==2)
			{	
				if(value0[0]>70&&value0[1]>70)
				{
					daotong1 =1;
					flag = 3;
					
				}	
					else
					{
						flag = 1;
						num = 0;
					}
			}
			if(num>3)
			{
				if(num<100)
				{
					if(value0[num-1]<-70)
					{
						daotong2 = 1;
					}
					
				}
				if(num>=100)
				{
					daoda1s = 1;
					flag = 1;
					num = 0;
					valuelt = value;
					kaishi  = 0;
				}
			}	

		}
		
		if(daotong1&&daotong2&&daoda1s)
		{
			flag = 2;
			daotongdaotong = 1;
			showf = 0;
		}
			
			                   
		}
        
}



//50ms触屏复位
void  TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  	
		{

		//	TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); 	
			
			tp_dev.scan(0);
			if((tp_dev.sta))         
			{
           
				if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
				{
                       if(tp_dev.x[0]>10&&tp_dev.x[0]<790&&tp_dev.y[0]<180&&tp_dev.y[0]>10)              
						showf = 1;
                 
				}
				//else flag = 0;
           TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );
			}
		}
}

