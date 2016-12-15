/*
 * motor.h
 *
 *  Created on: 14. 12. 2016
 *      Author: Jaroslav Duraj
 */

#ifndef MOTOR_H_
#define MOTOR_H_

void set_recv_data();

void EasyStepper(void);
void SensorPinInit(void);
uint8_t Sensor(void);
void SetAngle(float Angle);
void StepsAuto(void);
void StepsManual(void);
void Initialize(void);
int count_of_steps(uint8_t krokovanie);
void set_RST_Pin(void);
void initRST_Pin(void);
void initDIR1_Pin(void);
void setDir(int dir);
void SetCenterAuto(void);

#endif /* MOTOR_H_ */
