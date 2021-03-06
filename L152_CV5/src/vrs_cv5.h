#include <stm32l1xx_gpio.h>
#include <stm32l1xx_rcc.h>
#include <stm32l1xx_adc.h>
#include <stm32l1xx_usart.h>
#include <stdbool.h>
#include <stdio.h>

void USART1_config(void);
void ADC_Measure(uint16_t AD_Value, bool change);
void ADC_Config(void);
void OutString(void);
