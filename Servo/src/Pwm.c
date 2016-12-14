/*
 * Pwm.c
 *
 *  Created on: 14. 12. 2016
 *      Author: Jaroslav Duraj
 */
#include "Pwm.h"
#include "main.h"

TIM_TimeBaseInitTypeDef TIM_9_TimeBaseStructure;
RCC_ClocksTypeDef RCC_Clocks;

extern int Speed;

void Timer9_Config(int period){
	RCC_GetClocksFreq(&RCC_Clocks);
	TIM_9_TimeBaseStructure.TIM_Period = period - 1;
	TIM_9_TimeBaseStructure.TIM_Prescaler = (RCC_Clocks.PCLK2_Frequency/1000000) - 1;
	TIM_TimeBaseInit(TIM9, &TIM_9_TimeBaseStructure);
}

void Timer9_Disable(void){
	EnableDisable(0);// Disable
	TIM_Cmd(TIM9, DISABLE);
	TIM_ITConfig(TIM9, TIM_IT_Update, DISABLE);
}

void Timer9_Enable(void){
	Speed = 200;
	EnableDisable(1);// Enable
	TIM_Cmd(TIM9, ENABLE);
	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
}


void Timer9_Initialize(int period){

	RCC_GetClocksFreq(&RCC_Clocks);

	/* TIM9 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	/* Time base configuration */
	TIM_9_TimeBaseStructure.TIM_Period = period - 1;
	TIM_9_TimeBaseStructure.TIM_Prescaler = (RCC_Clocks.PCLK2_Frequency/1000000) - 1;
	TIM_9_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_9_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9, &TIM_9_TimeBaseStructure);
	TIM_Cmd(TIM9, ENABLE);
	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM9_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);

}

void initPWM1_Pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
