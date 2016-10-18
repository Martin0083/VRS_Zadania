#include <stm32l1xx_gpio.h>
#include <stm32l1xx_rcc.h>
#include <stm32l1xx_adc.h>
#include <stm32l1xx_usart.h>
#include <stm32l1xx_tim.h>
#include <stm32l1xx_exti.h>
#include <stm32l1xx_dma.h>
#include <stdio.h>

uint16_t AD_Value;
uint8_t Namerane;

void GPIO_CONFIG(void);
void Nvic_Config(void);
void ADC_Measure_Transmit(uint16_t AD_Value);
void ADC_Config(void);
void OutString(void);
