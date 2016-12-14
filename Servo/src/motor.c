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

extern uint16_t recv_speed;
extern float recv_angle;
extern uint16_t recv_stepping;
extern float recv_start_angle;
extern float recv_end_angle;
extern uint16_t recv_mode;

long Steps = 0;
long SetSteps = 0;
extern int Speed;
uint8_t Init = 0;
uint8_t Auto = 2; //1 = Automat, 0 = Manual, 2 = NOP
uint8_t Finish;
extern int MaxSteps;



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

// Spusti s alen raz a to na zaciatku, akonahle vrati funkcia Sensor hodnotu 1, motor je na 90° a to je pociatocna poloha
void Initialize(void){
	setDir(1); // Set any direction
	Finish = 0;

	if(Sensor()){
		Init = 1;
		Steps = MaxSteps/2; // 180°
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

		if(Steps >= MaxSteps || Steps <= 0){
			Speed = 200;
			Timer9_Config(Speed);
		}

		if(!(Steps%50)){
			Speed -=1;
			Timer9_Config(Speed);
		}
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
void SetAngle(float Angle){

	if(Auto == 0){
		SetSteps = round((Angle*MaxSteps)/360);

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
	float dlzka_kroku = 1.8;//1.8°


	switch(krokovanie){
		case 4:
			return MaxKrokovanie = 360/(dlzka_kroku/16)*2;
		case 3:
			return MaxKrokovanie = 360/(dlzka_kroku/8)*2;
		case 2:
			return MaxKrokovanie = 360/(dlzka_kroku/4)*2;
		case 1:
			return MaxKrokovanie = 360/(dlzka_kroku/2)*2;
		case 0:
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
		if(!Auto)// manual mode
		{
			//recv_speed = received_data[1];
			spi_set_step_mode(recv_stepping);
			MaxSteps = count_of_steps(recv_stepping);
			SetAngle(recv_angle);
		}
		else // auto mode
		{
			//recv_speed = received_data[1];
			//recv_start_angle = received_data[2];
			//recv_end_angle = received_data[3];
			spi_set_step_mode(recv_stepping);
			MaxSteps = count_of_steps(recv_stepping);
		}
	}

}
