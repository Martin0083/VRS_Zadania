/*
 * Com_config.c
 *
 *  Created on: 22. 11. 2016
 *      Author: Martin
 */

#include <Com.h>

TIM_TimeBaseInitTypeDef TIM_9_TimeBaseStructure;
RCC_ClocksTypeDef RCC_Clocks;

long Steps = 0;
long SetSteps = 0;
extern int Speed;
uint8_t Init = 0;
uint8_t Auto = 2; //1 = Automat, 0 = Manual, 2 = NOP
uint8_t Finish;
extern int MaxSteps;

void initSPI1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	//PA5 - SPI1_SCK
	//PA6 - SPI1_MISO
	//PA7 - SPI1_MOSI
	//PA4 - SPI1_NSS (SPI1_CS)

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = SPI_CRC_Rx;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;


	/* Apply SPI configuration after enabling it */
	SPI_Init(SPI1, &SPI_InitStructure);
	/* SPI Peripheral Enable */
	SPI_Cmd(SPI1, ENABLE);
}

void initCS_Pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void initRST_Pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
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

void initDIR1_Pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void SensorPinInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Sensor(void){

	return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) != (uint8_t)Bit_RESET);

//	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) != (uint8_t)Bit_RESET){
//	  return 1; // Snimac preruseny
//	}else{
//	  return 0; // Snimac volny
//	}
}

void setDir(int dir){
	if(dir == 1){
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
	}else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	}
}

void set_RST_Pin(void)
{
	GPIOA->BSRRL = GPIO_Pin_9;

}

uint8_t  WriteSPI1(uint8_t  txAddr, uint8_t  txData)
{

 uint8_t rxData;

 device_Select();

 SPI1->DR = txAddr;
 // wait until TXE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
 // wait until RXNE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
 // read the rx buff to clear the RXNE flag (garbage)
 rxData = SPI1->DR;

 device_Unselect();
 //Delay(0);
 device_Select();

 SPI1->DR = txData;
 // wait until TXE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
 // wait until RXNE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
 // read the rx buff to clear the RXNE flag (garbage)
 rxData = SPI1->DR;

 device_Unselect();
 //Delay(0);

 return rxData;
}

uint8_t ReadSPI1(uint8_t  txAddr)
{

 uint8_t   rxData;

 device_Select();

 SPI1->DR = txAddr|0b00100000;
 // wait until TXE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
 // wait until RXNE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
 // read the rx buff to clear the RXNE flag (garbage)
 rxData = SPI1->DR;

 device_Unselect();
 //Delay(0);
 device_Select();

 SPI1->DR = 0b00000000;
 // wait until TXE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
 // wait until RXNE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
 // read the rx buff to clear the RXNE flag (garbage)
 rxData = SPI1->DR;

 device_Unselect();
 //Delay(0);

 return rxData;
}

uint8_t EnableDisable(int state)
{

 uint16_t  rxData;
 uint16_t  txData;

 if(state == 1){
  txData = 0b10111000;//enable
 }else{
  txData = 0b10101000;//disable
 }

 device_Select();


 SPI1->DR = txData;
 //wait until TXE = 1;
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
 //wait until RXNE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
 //read the rx buff to clear the RXNE flag (garbage)
 rxData = SPI1->DR;

 device_Unselect();
 //Delay(0);

 return rxData;
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

// Spustanie funkcii v zavyslosti na zvolenom mode, tato funkcia zbieha v casovaci
void EasyStepper(){

	GPIO_ToggleBits(GPIOC, GPIO_Pin_7);//PWM Generation


	if(!Init){
		Initialize();
	}

	if(Auto == 1){
		if(Init){
			StepsAuto();
		}
	}

	if(Auto == 0){
		if(Init){
			StepsManual();
		}
	}
}

// Spusti s alen raz a to na zaciatku, akonahle vrati funkcia Sensor hodnotu 1, motor je na 90� a to je pociatocna poloha
void Initialize(void){
	setDir(1); // Set any direction
	Finish = 0;

	if(Sensor()){
		Init = 1;
		Steps = MaxSteps/2; // 90�
		Finish = 1;
		Timer9_Disable();
	}
}

// Automaticky mode, spusti sa ak je premenna Auto == 1
void StepsAuto(void){
	if(Init){
		if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8))//direction
		{
			Steps--; // ACLKW
		}else{
			Steps++; // CLKW
		}

		if(Steps >= MaxSteps){
			GPIO_ToggleBits(GPIOA, GPIO_Pin_8);
		}
		if(Steps == 0){
			GPIO_ToggleBits(GPIOA, GPIO_Pin_8);
		}

//		if(Steps >= MaxSteps || Steps <= 0){ // pomocou tohto sa ot��a nekon�tantnou r�chlos�ou
//			Speed = 200;
//			Timer9_Config(Speed);
//		}
//
//		if(!(Steps%50)){
//			Speed -=1;
//			Timer9_Config(Speed);
//		}
	}
}

// manualny mod, ak jre premenna Auto == 0 tak funkcia otoci motor na zelanu poziciu ktoru nastavila funkcia SetAngle
void StepsManual(void){
	if(Init){
		Finish = 0;
		if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8)){
			Steps--; // ACLKW
		}else{
			Steps++; // CLKW
		}

		if(!(Steps%50)){
			Speed -=1;
			Timer9_Config(Speed);
		}

		if(Steps == SetSteps){
			Finish = 1;
			Timer9_Disable();
		}
	}
}

// Funkcia prepocita zelany uhol na pocet krokov a nastavy smer
void SetAngle(int Angle){

	if(Auto == 0){
		SetSteps = round((Angle*MaxSteps)/180);

		if(SetSteps > Steps){
			setDir(0); // CLKW
			Timer9_Enable();
			Timer9_Config(200);
		}
		if(SetSteps < Steps){
			setDir(1); // ACLKW
			Timer9_Enable();
			Timer9_Config(200);
		}
		if(SetSteps == Steps){
			Timer9_Disable();
		}
	}
}

// funkcia vracia Maximalny pocet krokov v zavyslosti na zvolenom mikrokrokovani, koli vypoctu polohy
int count_of_steps(uint8_t krokovanie){
	int MaxKrokovanie = 0;

	switch(krokovanie){
		case 4:
			return MaxKrokovanie = 1600*2;
		case 3:
			return MaxKrokovanie = 1600;
		case 2:
			return MaxKrokovanie = 1600/2;
		case 1:
			return MaxKrokovanie = 1600/4;
		default:
			return 0;
	}
}

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

void device_Select(void)
{
	GPIOB->BSRRH = GPIO_Pin_6;
}

void device_Unselect(void)
{
	GPIOB->BSRRL = GPIO_Pin_6;
}

