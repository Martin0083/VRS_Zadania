#include <main.h>

void GPIO_CONFIG(void){

	GPIO_InitTypeDef GPIO_STRUCT_A;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_STRUCT_A.GPIO_Pin  = GPIO_Pin_5;
	GPIO_STRUCT_A.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_STRUCT_A.GPIO_OType = GPIO_OType_PP;
	GPIO_STRUCT_A.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_STRUCT_A.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_STRUCT_A);
}

void Timer9_Initialize(void){

	TIM_TimeBaseInitTypeDef TIM_9_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);

	TIM_9_TimeBaseStructure.TIM_Period = 1;
	TIM_9_TimeBaseStructure.TIM_Prescaler = 31000;
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

void Timer9_Period_Set(uint16_t Period){

	TIM_TimeBaseInitTypeDef TIM_9_TimeBaseStructure;

	TIM_Cmd(TIM9, DISABLE);
	TIM_ITConfig(TIM9, TIM_IT_Update, DISABLE);

	TIM_9_TimeBaseStructure.TIM_Period = Period-1;
	TIM_9_TimeBaseStructure.TIM_Prescaler = 32000;
	TIM_9_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_9_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9, &TIM_9_TimeBaseStructure);

	TIM_Cmd(TIM9, ENABLE);
	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);

}

void adc_init(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);//Opravit a upravit

	/* Configure ADCx Channel 2 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; // upravit pin podla potreby
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);

	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	/* Enable ADC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADCx regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_16Cycles);

	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{
	}

	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv(ADC1);
}

uint16_t ADC_Conversion(void){
	uint16_t AD_value = 0;
	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv(ADC1);

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}

	AD_value = ADC_GetConversionValue(ADC1);

	return AD_value;
}


