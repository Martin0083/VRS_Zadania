/*
 * vrs_cv5.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Jaroslav Duraj
 */

#ifndef USART_H_
#define USART_H_

void adc_init(void);
void UART1_init(void);
void USART1_IRQHandler(void);
void ADC1_IRQHandler(void);
void send_data(void);
void fill_buffer(void);
void parse_recv_data(void);

#endif /* USART_H_ */
