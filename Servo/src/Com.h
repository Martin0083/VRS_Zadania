/*
 * Com_config.h
 *
 *  Created on: 22. 11. 2016
 *      Author: Martin
 */

#ifndef COM_H_
#define COM_H_

#include <main.h>

void initSPI1(void);
void initPWM1_Pin(void);
uint8_t  WriteSPI1(uint8_t  txAddr, uint8_t  txData);
uint8_t  ReadSPI1(uint8_t  txAddr);
uint8_t  EnableDisable(int  state);
void Timer9_Initialize(int period);
void initCS_Pin(void);
void device_Select(void);
void device_Unselect(void);
void set_RST_Pin(void);
void initRST_Pin(void);
void initDIR1_Pin(void);
void setDir(int dir);
void Timer9_Disable(void);
void Timer9_Enable(void);
void Timer9_Config(int period);
void EasyStepper(void);
void SensorPinInit(void);
uint8_t Sensor(void);
void SetAngle(int Angle);
void StepsAuto(void);
void StepsManual(void);
void Initialize(void);
int count_of_steps(uint8_t krokovanie);

#endif /* COM_H_ */
