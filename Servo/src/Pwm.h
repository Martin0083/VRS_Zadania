/*
 * Pwm.h
 *
 *  Created on: 14. 12. 2016
 *      Author: Jaroslav Duraj
 */

#ifndef PWM_H_
#define PWM_H_

void Timer9_Disable(void);
void Timer9_Enable(void);
void Timer9_Config(int period);
void Timer9_Initialize(int period);
void initPWM1_Pin(void);

#endif /* PWM_H_ */
