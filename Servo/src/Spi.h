/*
 * Spi.h
 *
 *  Created on: 14. 12. 2016
 *      Author: Jaroslav Duraj
 */

#ifndef SPI_H_
#define SPI_H_
#include "stdio.h"

void spi_set_current(uint8_t current);
void spi_set_step_mode(uint8_t step_mode);
void initSPI1(void);
uint8_t  WriteSPI1(uint8_t  txAddr, uint8_t  txData);
uint8_t  ReadSPI1(uint8_t  txAddr);
uint8_t  EnableDisable(int  state);
void initCS_Pin(void);
void device_Select(void);
void device_Unselect(void);

#endif /* SPI_H_ */
