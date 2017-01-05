/*
 * motor.h
 *
 *  Created on: 14. 12. 2016
 *      Author: Jaroslav Duraj
 */

#ifndef MOTOR_H_
#define MOTOR_H_

void set_recv_data(); //funkcia na nastavenie prijatých dát z USART

void EasyStepper(void); //funkcia ktorá je volaná v TIM9_IRQHandler
void SensorPinInit(void); //inicializácia pinu pre opitckú závora
uint8_t Sensor(void); //funkcia na èítanie hodnoty pinu na ktorom je pripojená optická závora
void SetAngle(float Angle); //funkcia na nastavenie uhlu
void StepsAuto(void); //funkcia vykonávaná ak je nastavený mód Auto
void StepsManual(void); //funkcia vykonávaná ak je nastavený mód manual
void SetToInitPosition(void); //funkcia na nastavenie páky motora do inicializaènej polohy(na optickej závore)
int count_of_steps(uint8_t krokovanie); // funkcia vracia Maximalny pocet krokov v zavislosti na zvolenom mikrokrokovani
void set_RST_Pin(void); //funkcia na setnutie RST pinu
void initRST_Pin(void); //funkcia na inicializáciu RST pinu
void initDIR1_Pin(void); //funkcia na inicializáciu DIR1 pinu, ktorý slúži na nastavenie smeru otáèania
void setDir(int dir); //funkcia na nastavenie smeru otáèania
void SetStartPositionAuto(void); //funkcia ktorá sa spušta v EasyStepper a slúži na nastavenie hraniènej(štartovacej)polohy nových hranièných poloh pre auto mód

#endif /* MOTOR_H_ */
