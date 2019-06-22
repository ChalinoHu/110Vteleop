
#include "stm32f10x_tim.h"
#include "hwtimer.h"
#include "lcd.h"
#include "adc.h"
#include "touch.h"

int count = 0;
int value,valuelt;

int value0[40],value1[3];

u8 flag1 = 1;	//�״δ򿪸ߵ�ƽ��־λ

int num = 0;  //�洢��ѹ�±�

u8 flag = 0;  ///״̬��־λ

u8 daotong1=0,daotong2=0,daoda1s=0;   //����ʱ���־λ

u8 daotongdaotong = 0; //���յ�ͨ��־λ

u8 showf = 1;		//��ʾ��־λ

u8 kaishi = 0,kaishi1 = 0;



// 25ms�ɼ�AD
void TIM3_Int_Init(void)
{
 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 99; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx				//���ݺ�������򿪻��߹رն�ʱ��	 
}


void TIM4_Int_Init(void)
{
 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 499; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx				//���ݺ�������򿪻��߹رն�ʱ��	 
}



//10ms��һ��AD
void  TIM3_IRQHandler(void)   //TIM3�ж�
{
			                   
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  	//���TIM3�����жϷ������
		{
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); 	 //���TIMx�����жϱ�־ 		 
			
			value = ADC_con();
		
			if(value>-70&&value<70) flag = 1;  //����
			
			if(value<-70)
			{
				flag = 0;	//�Ͽ�
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



//50ms������λ
void  TIM4_IRQHandler(void)   //TIM4�ж�
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

