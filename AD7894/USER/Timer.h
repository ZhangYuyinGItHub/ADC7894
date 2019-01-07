
#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

#define START_TIME  gTimerCnt=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

extern volatile u32 gTimerCnt;

void TIM2_NVIC_Configuration(void);
void TIM2_Configuration(void);

#endif
