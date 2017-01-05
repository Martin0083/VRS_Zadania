/*
 * motor.c
 *
 *  Created on: 14. 12. 2016
 *      Author: Jaroslav Duraj
 */

#include "stdio.h"
#include "motor.h"
#include "main.h"
#include "Spi.h"
#define START_SPEED 30

extern uint16_t recv_speed;
extern float recv_angle;
extern uint16_t recv_stepping;
extern uint16_t recv_stepping_old;
extern float recv_start_angle;
extern float recv_end_angle;
extern uint16_t recv_mode;
extern int MaxSteps;

float krokovanieZlomok = 0;
long Steps = 0;
long SetSteps = 0;
uint8_t Init = 0;
uint8_t Auto = 2; //1 = Automat, 0 = Manual, 2 = NOP
uint8_t Finish;
uint8_t SetCenterAutoInit = 0;
uint8_t MicroSteppingChange = 0;

uint8_t SetAngleFinished = 1;
uint16_t manual_period_speed = 2000;

uint16_t start_step = 0;//30/360*MaxSteps; 0
uint16_t end_step = 0;//330/360*MaxSteps; 0

uint32_t interupt_counter_auto = 0;
uint32_t change_speed = 10000; //change_speed=1 znamen· zmena r˝chlosti kaûd˝ch 100 us  //100*10000=zmena kaûd˝ch 1000 000 us= 1s
uint16_t autoModePeriod = 100; // 100us
uint16_t TimeOfToggle = START_SPEED; //jeden krok za 100*100*2 us = 20ms
uint16_t TimeOfToggleDecrement = 4; //toto ËÌslo znamen· o koæko sa bude zmenöovaù r˝chlosù kaûd˝ch 1 s
									//ak je TimeOfToggleDecrement=1, tak 1*100*2 us = 0,2ms
									//ak je TimeOfToggleDecrement=2, tak 2*100*2 us = 0,4ms
									//							 =5,     5*100*2 us = 1000us = 1 ms. Takûe kaûd˙ 1s sa zmenöÌ Ëas jednÈho kroku o 1ms
double TimeOfToggleDecrement_double = 0;

// Spustanie funkcii v zavyslosti na zvolenom mode, tato funkcia zbieha v casovaci
void EasyStepper(){

	if(!Auto || !Init) //for setting of speed in auto Mode we dont use this Toggle, but we use Toggle in function of auto mode
	{
		GPIO_ToggleBits(GPIOC, GPIO_Pin_7);//PWM Generation
	}

	if(!Init){
		SetToInitPosition();
	}

	if(Init){
		if(Auto == 1){
			if(MicroSteppingChange){
				MicroSteppingChange = 0;
			}
			StepsAuto();
		}

		if(Auto == 0){
			if(MicroSteppingChange){
				SetAngle(recv_angle);
				MicroSteppingChange = 0;
			}
			StepsManual();
		}
	}
}

// Spusti s alen raz a to na zaciatku, akonahle vrati funkcia Sensor hodnotu 1, motor je na 90∞ a to je pociatocna poloha
void SetToInitPosition(void){

	start_step = recv_start_angle*MaxSteps/360;
	end_step = recv_end_angle*MaxSteps/360;

	setDir(1); // Set any direction
	Finish = 0;

	if(Sensor()){
		Init = 1;
		recv_stepping_old = recv_stepping;
		Steps = MaxSteps/2; // 180∞
		Finish = 1;
	}
}

// Automaticky mode, spusti sa ak je premenna Auto == 1
void StepsAuto(void){
	if(SetAngleFinished){

		interupt_counter_auto++; //100us
		if(interupt_counter_auto>=change_speed)//
		{
			TimeOfToggle = TimeOfToggle - TimeOfToggleDecrement;
			interupt_counter_auto = 0;
		}

		if(TimeOfToggle==0)
		{
			TimeOfToggle = 1;
		}
		if(!(interupt_counter_auto%TimeOfToggle))
		{
			if(Steps >= end_step || Steps <= start_step){
				interupt_counter_auto = 0;
				TimeOfToggle = START_SPEED;
				GPIO_ToggleBits(GPIOA, GPIO_Pin_8); //zmena smeru
			}

			GPIO_ToggleBits(GPIOC, GPIO_Pin_7);//PWM Generation

			if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8))//direction
			{
				Steps--; // ACLKW
			}else{
				Steps++; // CLKW
			}
		}
	}
	else
	{
		GPIO_ToggleBits(GPIOC, GPIO_Pin_7);//PWM Generation
		SetStartPositionAuto();
	}
}

// manualny mod, ak jre premenna Auto == 0 tak funkcia otoci motor na zelanu poziciu ktoru nastavila funkcia SetAngle
void StepsManual(void){
	Finish = 0;

	if(Steps == SetSteps){
		Finish = 1;
		Timer9_Disable();
	}else{
		if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8)){
			Steps--; // ACLKW
		}else{
			Steps++; // CLKW
		}
	}
}

//funkcia ktor· sa spuöta v EasyStepper a sl˙ûi na nastavenie hraniËnej(ötartovacej)polohy nov˝ch hraniËn˝ch poloh pre auto mÛd
void SetStartPositionAuto(void){

	if(Steps == SetSteps){
			SetAngleFinished = 1; // Nachadza sa v krajnej polohe
			Timer9_Config(autoModePeriod);
	}else{
		if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8)){
				Steps--; // ACLKW
			}else{
				Steps++; // CLKW
			}
	}
}

// Funkcia prepocita zelany uhol na pocet krokov a nastavy smer
void SetAngle(float Angle){

	SetSteps = round((Angle*MaxSteps)/360);

	if(SetSteps > Steps){
		setDir(0); // CLKW
		Timer9_Enable();
		Timer9_Config(manual_period_speed);

	}
	if(SetSteps < Steps){
		setDir(1); // ACLKW
		Timer9_Enable();
		Timer9_Config(manual_period_speed);
	}
	if(SetSteps == Steps){
		if(!Auto)
		{
			Timer9_Disable();
		}
	}
}

// funkcia vracia Maximalny pocet krokov v zavislosti na zvolenom mikrokrokovani, koli vypoctu polohy
int count_of_steps(uint8_t krokovanie){
	int MaxKrokovanie = 0;
	float dlzka_kroku = 1.8;//1.8∞


	switch(krokovanie){
		case 4:
			krokovanieZlomok = 0.0625; //1/16;
			return MaxKrokovanie = 360/(dlzka_kroku/16)*2;
		case 3:
			krokovanieZlomok = 0.125;//1/8;
			return MaxKrokovanie = 360/(dlzka_kroku/8)*2;
		case 2:
			krokovanieZlomok = 0.25;//1/4;
			return MaxKrokovanie = 360/(dlzka_kroku/4)*2;
		case 1:
			krokovanieZlomok = 0.5;//1/2;
			return MaxKrokovanie = 360/(dlzka_kroku/2)*2;
		case 0:
			krokovanieZlomok = 1;
			return MaxKrokovanie = 360/dlzka_kroku*2;
		default:
			return 0;
	}
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


void set_recv_data()
{
	if(Init && Finish)
	{
		Auto = recv_mode;
		if(recv_stepping != recv_stepping_old){
			Init = 0;
			MicroSteppingChange = 1;
		}
		if(!Auto)// manual mode
		{
			spi_set_step_mode(recv_stepping);
			MaxSteps = count_of_steps(recv_stepping);
			manual_period_speed = (krokovanieZlomok*150000)/(recv_speed*2); // v˝znam konötanty 150 000 je vysvetlen˝ v dokument·ciÌ
			SetAngle(recv_angle);
		}
		else // auto mode
		{
			spi_set_step_mode(recv_stepping);
			MaxSteps = count_of_steps(recv_stepping);
			start_step = recv_start_angle*MaxSteps/360;
			end_step = recv_end_angle*MaxSteps/360;

			TimeOfToggleDecrement_double=round(recv_speed/2);//ak je v GUI nastavenÈ 0.2ms, prijmem ËÌslo 2. MusÌm to podeliù 10 a vyn·sobiù (1/0.2) = recv_speed/2
			TimeOfToggleDecrement = TimeOfToggleDecrement_double;

			SetAngleFinished = 0;//nastav krajn˙ polohu novÈho novÈho rozsahu
			SetCenterAutoInit = 0; // znova nastav motor do inicializacnej(180∞) polohy
			MicroSteppingChange = 1; // aj ked sa microstepping nezmenil je potrebne znova nastavit stred, ked je tato premenna na 1 tak sa pred spustenim auto rezimu nastavi na stred noveho rozsahu
			Init = 0;
			SetAngle(recv_start_angle);
		}
	}
}
