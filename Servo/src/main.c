/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 6.0.0   2016-11-22

The MIT License (MIT)
Copyright (c) 2009-2016 Atollic AB

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************
*/

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"

#include "main.h"
/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
int txData;
int rxData;
extern long Steps;
extern uint8_t Init;
extern uint8_t Auto;
extern uint8_t Finish;
extern uint8_t tim9_enable;
extern uint16_t end_step;
float period_Speed = 150;
int Speed_dir = 0;
int MaxSteps = 0;
uint8_t krokovanie = 4; //4: 1/16, 3: 1/8, 2: 1/4, 1: 1/2, 0: full step

/* Private function prototypes */
/* Private functions */


/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
  int i = 0;
  SystemInit();

  initSPI1();

  initCS_Pin(); // chip select pin

  SensorPinInit();

  initRST_Pin();
  set_RST_Pin();

  device_Unselect();

  spi_set_current(5);//Nastavenie prudu Tval:5x31.25mA
  spi_set_step_mode(krokovanie); // nastavenie step_mode

  MaxSteps = count_of_steps(krokovanie);
  end_step = MaxSteps;

  initDIR1_Pin();
  setDir(1);
  initPWM1_Pin();
  Timer9_Initialize(period_Speed); // us
  UART3_init();

  Auto = 1;

  /* Infinite loop */
  while (1)
  {
	  if(Auto && !tim9_enable)
	  {
		  Timer9_Enable();
	  }


	  // toto je tu len koli testovaniu
	  // Funkcia SetAngle moze zbiehat len vtedy ak je pohyb ukonceny, a nesmie zbiehat viac krat po sebe
	  /*zac:
	  if(Init && Finish && !Auto){
		  if(i == 0){
			  Auto = 0;
			  SetAngle(180);
			  i++;
			  goto zac;
		  }
		  if(i == 1){
			  Auto = 0;
			  SetAngle(45);
			  i++;
			  goto zac;
		  }
		  if(i == 2){
			  Auto = 0;
			  SetAngle(90);
			  i++;
			  goto zac;
		  }
	  }*/

//	  if(!(Steps%100)){
//		  Timer9_Config(period_Speed);
//
//		  if(Speed_dir){
//			  period_Speed -=1;
//		  }else{
//			  period_Speed +=1;
//		  }
//	  }
//	  if(period_Speed <= 50 || period_Speed >= 300){
//		  if(Speed_dir){
//			  Speed_dir = 0;
//		  }else{
//			  Speed_dir = 1;
//		  }
//	  }

	  //rxData = ReadSPI1(0x09);
	  //rxData = ReadSPI1(0x16);
  }
  return 0;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
