#include <vrs_cv5.h>

char AD_text_value[6];

void GPIO_CONFIG(void){

	GPIO_InitTypeDef GPIO_STRUCT_A;
	GPIO_InitTypeDef GPIO_ADC_InitStructure;
	GPIO_InitTypeDef GPIO_UART1_InitStructure;
	USART_InitTypeDef USART1_InitStructure;

	/* Led pin configuration */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_STRUCT_A.GPIO_Pin  = GPIO_Pin_5;
	GPIO_STRUCT_A.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_STRUCT_A.GPIO_OType = GPIO_OType_PP;
	GPIO_STRUCT_A.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_STRUCT_A.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_STRUCT_A);

	/* Configure ADC1 Pin1 as analog input */
	GPIO_ADC_InitStructure.GPIO_Pin = GPIO_Pin_1; // upravit pin podla potreby
	GPIO_ADC_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_ADC_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_ADC_InitStructure);

	/* Configure pins PA9: TX PA10: RX*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_UART1_InitStructure.GPIO_Pin  = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_UART1_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_UART1_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_UART1_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_UART1_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_UART1_InitStructure);

	/* Set PA9 PA10 as alternate*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* Configure UART1 */
	USART1_InitStructure.USART_BaudRate = 9600;
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	USART1_InitStructure.USART_Parity = USART_Parity_No;
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART1_InitStructure);

	/* Enable interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* Enable USART1 */
	USART_Cmd(USART1, ENABLE);

	//USART_ClearFlag(USART1, USART_FLAG_TC);

}

void Nvic_Config(void){

	NVIC_InitTypeDef NVIC_ADC1_InitStructure;
	NVIC_InitTypeDef NVIC_USART1_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_ADC1_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam preru�en� n�jdete v s�bore stm32l1xx.h
	NVIC_ADC1_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_ADC1_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_ADC1_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_ADC1_InitStructure);

	NVIC_USART1_InitStructure.NVIC_IRQChannel = USART1_IRQn; //zoznam preru�en� n�jdete v s�bore stm32l1xx.h
	NVIC_USART1_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
	NVIC_USART1_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_USART1_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_USART1_InitStructure);
}

void ADC_Config(void){

	ADC_InitTypeDef ADC_InitStructure;

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
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* Enable interrupt */
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_ITConfig(ADC1, ADC_IT_OVR, ENABLE);

	/* ADC1 regular channel1 configuration */
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

void ADC_Measure_Transmit(uint16_t AD_Value){

	double Voltage = 0;

	for(int i=0; i<=5; i++){
		AD_text_value[i] = '0';
	}

	Voltage = (AD_Value/4096.0)*3.3;

	sprintf(AD_text_value, "%.2f\r\n", Voltage);
	OutString(); // odosielanie nejde
}

void OutString()
{
  char *s=AD_text_value;

  for(int index = 0; index < 6; index++)
  {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}; // Wait for Empty
    USART_SendData(USART1, *s++); // Send Char
  }

}
