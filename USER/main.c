#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24cxx.h"
#include "flash.h"
#include "touch.h"
#include "adc.h"
#include "hwtimer.h"

//C库
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

extern u8 temp;
extern int value;
extern u8 showf;
extern u8 num;
extern u8 daotongdaotong;

extern u8 daotong1;
extern u8 daotong2;
extern u8 daoda1s;
extern int valuelt;


int sum;
u8 j;


extern u8 flag;   // 0	断开
                  // 1	故障
									// 2	导通
 void hardware_init()
 {
	
	KEY_Init();	
	delay_init();	   
	NVIC_Configuration(); 
	  
	LCD_Init();
	tp_dev.init();
    Adc_Init();     
 	LED_Init();
	TIM3_Int_Init();	

 }
 int fitter()
 {
	for(j = 0;j<5;j++)
	 {
		 sum +=value;
	 }
	 if(j==5) j=0;
	 
	 return sum/5;

 }

 
 int main(void)
 {	 	

	 u8 n=0; 
	 u8 i;
	 int valuecur;
	 
	 hardware_init();
	 
	 		
 	 POINT_COLOR=DARKBLUE;
	 //电压
	 LCD_ShowHz_128_huan(100,165,0,0);  
	 LCD_ShowHz_128_huan(196,165,1,0);	
	 //状态
	 LCD_ShowHz_128_huan(100,293,2,0);
	 LCD_ShowHz_128_huan(196,293,3,0);
	 //冒号128x64    424
	 LCD_ShowHz_maohao(360,165,16,0);
	 LCD_ShowHz_maohao(360,293,16,0);
	 //VS
	 Show96x48VS(296,165,1,0);
	 Show96x48VS(296,293,0,0);
	
	 //超高电压输电公司曲靖局
		 for(n=0;n<10;n++)
	 {
		 LCD_ShowHz_48(150+50*n,56,n,0);
	 }
  
	TIM_Cmd(TIM3, ENABLE); 
	 
	delay_ms(50);
	 
   while(1)
       
   {
			tp_dev.scan(0);
			if((tp_dev.sta))         
			{
           
				if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
				{
                       if(tp_dev.x[0]>2&&tp_dev.x[0]<798&&tp_dev.y[0]<478&&tp_dev.y[0]>2)              
						
					   showf = 1;
					   tp_dev.sta = 0;
					   daotong1 = 0;
					   daotong2 = 0;
					   daoda1s = 0;
					   num  = 0;
					   daotongdaotong = 0;
					  valuelt = value;
					   if(value>70)
					   {
						flag = 1;
					   }
                 
				}
           
			}
	
			valuecur = fitter();
	   if(value>=0)
	  {	 
		  valuecur=-valuecur;
		  LCD_Fill(410,150,474,278,BACK_COLOR);
		if(value<10) //个位数
		{  
			LCD_Fill(474,150,602,278,BACK_COLOR);
			LCD_Fill(666,150,800,278,BACK_COLOR);
			LCD_printfnum128(602,150,256,128,"%d",valuecur);
		}
		else if(value>99)
		{
			LCD_Fill(666,150,800,278,BACK_COLOR);
			LCD_printfnum128(474,150,256,128,"%d",valuecur);
		}
		else		//两位数
		{
			LCD_Fill(474,150,538,278,BACK_COLOR);
			LCD_printfnum128(538,150,256,128,"%d",valuecur);
			LCD_Fill(666,150,800,278,BACK_COLOR);			
		}

	  }
	  
	 
	  
	  
	 if(value<0) 
	 {	
		 Show128x64fuhao(410,150,0);	//128x64
	
		if(valuecur<-99)	//三位数
		{
			
			LCD_Fill(666,150,800,278,BACK_COLOR);
			LCD_printfnum128(474,150,256,128,"%d",-valuecur);
		}
		else if(valuecur>-10)	//个位数
		{
			LCD_Fill(474,150,602,278,BACK_COLOR);
			LCD_Fill(666,150,800,278,BACK_COLOR);
			LCD_printfnum128(602,150,256,128,"%d",-valuecur);
		}	
		else		//两位数
		{
			LCD_Fill(474,150,538,278,BACK_COLOR);
			LCD_printfnum128(538,150,256,128,"%d",-valuecur);
			LCD_Fill(666,150,800,278,BACK_COLOR);
		}			
	 
	 }
		 
		 
		 //断开
		 if(flag ==0)
		 {
			if(showf) //显示开关
			{
				// LCD_Fill(410,293,800,359,BACK_COLOR);
				LCD_ShowHz_128_huan(444,293,8,0);
				LCD_ShowHz_128_huan(544,293,9,0);
			//黄亮红灭
				GPIO_SetBits(GPIOF,GPIO_Pin_14);
				GPIO_ResetBits(GPIOF,GPIO_Pin_12);
			}
		 }
		 //故障
		 if(flag ==1)
		 {
			if(showf)
			{
			
			LCD_ShowHz_128_huan(444,293,6,0);
			LCD_ShowHz_128_huan(544,293,7,0);
			//红亮黄灭
			GPIO_SetBits(GPIOF,GPIO_Pin_12);
		    GPIO_ResetBits(GPIOF,GPIO_Pin_14);
			}
		 }

		 //导通
		 if(daotongdaotong)
		 {

		 
			//导通
			LCD_ShowHz_128_huan(444,293,4,0);
			LCD_ShowHz_128_huan(544,293,5,0);
			GPIO_SetBits(GPIOF,GPIO_Pin_14);
			GPIO_ResetBits(GPIOF,GPIO_Pin_12);
		 }
   }
}
 
 