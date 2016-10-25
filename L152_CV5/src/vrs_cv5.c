#include <vrs_cv5.h>
extern char AD_text_value[6];

int format = 0; // 0 = text 1 = cislo


void USART1_config(void){

	NVIC_InitTypeDef NVIC_USART1_InitStructure;
	GPIO_InitTypeDef GPIO_UART1_InitStructure;
	USART_InitTypeDef USART1_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* Set PA9 PA10 as alternate*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* Configure pins PA9: TX PA10: RX*/
	GPIO_UART1_InitStructure.GPIO_Pin  = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_UART1_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_UART1_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_UART1_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_UART1_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_UART1_InitStructure);

	/* Configure UART1 */
	USART1_InitStructure.USART_BaudRate = 9600;
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	USART1_InitStructure.USART_Parity = USART_Parity_No;
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART1_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_USART1_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_USART1_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
	NVIC_USART1_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_USART1_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_USART1_InitStructure);

	/* Enable interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE | USART_IT_TC, ENABLE);

	/* Enable USART1 */
	USART_Cmd(USART1, ENABLE);

}

void ADC_Config(void){

	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_ADC_InitStructure;
	NVIC_InitTypeDef NVIC_ADC1_InitStructure;

	/* Configure ADC1 Pin1 as analog input */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_ADC_InitStructure.GPIO_Pin = GPIO_Pin_1; // upravit pin podla potreby
	GPIO_ADC_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_ADC_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_ADC_InitStructure);

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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_ADC1_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam prerušení nájdete v súbore stm32l1xx.h
	NVIC_ADC1_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_ADC1_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_ADC1_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_ADC1_InitStructure);
}

void ADC_Measure(uint16_t AD_Value, bool change){
	double Voltage = 0;

	for(int i=0; i<=5; i++){
		AD_text_value[i] = '0';
	}

	if(change){
		if(format){
			format = 0;
		}else{
			format = 1;
		}
	}

	if(!format){
		Voltage = (AD_Value/4096.0)*3.3;
		sprintf(AD_text_value, "%.2f\r\n", Voltage);
	}else if(format){
		if(AD_Value < 1000){
			sprintf(AD_text_value, "0%d\r\n", AD_Value);
			if(AD_Value<100){
				sprintf(AD_text_value, "00%d\r\n", AD_Value);
				if(AD_Value < 10){
					sprintf(AD_text_value, "000%d\r\n", AD_Value);
				}
			}
		}else{
			sprintf(AD_text_value, "%d\r\n", AD_Value);
		}

	}
}

//void OutString()
//{
//  char *s=AD_text_value;
//
//  for(int index = 0; index < 6; index++)
//  {
//    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}; // Wait for Empty
//    USART_SendData(USART1, *s++); // Send Char
//  }
//
//}
