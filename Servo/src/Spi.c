/*
 * Spi.c
 *
 *  Created on: 14. 12. 2016
 *      Author: Jaroslav Duraj
 */
#include "Spi.h"
#include "stdio.h"
#include "main.h"


void spi_set_current(uint8_t current)
{
	EnableDisable(0);// Disable
	//WriteSPI1(0x09, 0b00000101);//adresa,data, Nastavenie prudu Tval:5x31.25mA
	WriteSPI1(0x09, current);//adresa,data, Nastavenie prudu Tval:5x31.25mA
	EnableDisable(1);// Enable
}

void spi_set_step_mode(uint8_t step_mode)
{
	EnableDisable(0);// Disable
	WriteSPI1(0x16, 0b10001000 | (step_mode & 0b111));//Step mode el pos: 7(000), step mode: full(000)
	EnableDisable(1);// Enable
}

