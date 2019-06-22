#ifndef _HWTIMER_H_
#define _HWTIMER_H_

#include "stm32f10x.h"


 extern int count,value0[40];

void TIM3_Int_Init(void);
void TIM4_Int_Init(void);
void TIM5_Int_Init(void);


#endif
