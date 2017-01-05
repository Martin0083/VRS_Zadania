/*
 * motor.h
 *
 *  Created on: 14. 12. 2016
 *      Author: Jaroslav Duraj
 */

#ifndef MOTOR_H_
#define MOTOR_H_

void set_recv_data(); //funkcia na nastavenie prijat�ch d�t z USART

void EasyStepper(void); //funkcia ktor� je volan� v TIM9_IRQHandler
void SensorPinInit(void); //inicializ�cia pinu pre opitck� z�vora
uint8_t Sensor(void); //funkcia na ��tanie hodnoty pinu na ktorom je pripojen� optick� z�vora
void SetAngle(float Angle); //funkcia na nastavenie uhlu
void StepsAuto(void); //funkcia vykon�van� ak je nastaven� m�d Auto
void StepsManual(void); //funkcia vykon�van� ak je nastaven� m�d manual
void SetToInitPosition(void); //funkcia na nastavenie p�ky motora do inicializa�nej polohy(na optickej z�vore)
int count_of_steps(uint8_t krokovanie); // funkcia vracia Maximalny pocet krokov v zavislosti na zvolenom mikrokrokovani
void set_RST_Pin(void); //funkcia na setnutie RST pinu
void initRST_Pin(void); //funkcia na inicializ�ciu RST pinu
void initDIR1_Pin(void); //funkcia na inicializ�ciu DIR1 pinu, ktor� sl��i na nastavenie smeru ot��ania
void setDir(int dir); //funkcia na nastavenie smeru ot��ania
void SetStartPositionAuto(void); //funkcia ktor� sa spu�ta v EasyStepper a sl��i na nastavenie hrani�nej(�tartovacej)polohy nov�ch hrani�n�ch poloh pre auto m�d

#endif /* MOTOR_H_ */
