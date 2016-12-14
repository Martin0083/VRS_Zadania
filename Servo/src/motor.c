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
extern uint16_t recv_angle;
extern uint16_t recv_stepping;
extern uint16_t recv_start_angle;
extern uint16_t recv_end_angle;
extern uint16_t recv_mode;

uint8_t Auto;

void set_recv_data()
{
	Auto = recv_mode;
	if(!Auto)//mode manual
	{
		//recv_speed = received_data[1];
		SetAngle(recv_angle);
		spi_set_step_mode(recv_stepping);
	}
	else //mode auto
	{
		//recv_speed = received_data[1];
		//recv_start_angle = received_data[2];
		//recv_end_angle = received_data[3];
		spi_set_step_mode(recv_stepping);
	}
}
