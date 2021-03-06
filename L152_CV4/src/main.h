#include <stm32l1xx_gpio.h>
#include <stm32l1xx_rcc.h>
#include <stm32l1xx_adc.h>
#include <stm32l1xx_usart.h>
#include <stm32l1xx_tim.h>
#include <stm32l1xx_exti.h>
#include <stm32l1xx_dma.h>
#include <stdio.h>

void GPIO_CONFIG(void);
void Timer9_Initialize(void);
void adc_init(void);
uint16_t ADC_Conversion(void);
void Timer9_Period_Set(uint16_t Period);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
