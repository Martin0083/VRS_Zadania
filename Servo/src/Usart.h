/*
 * vrs_cv5.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Jaroslav Duraj
 */

#ifndef USART_H_
#define USART_H_

void UART3_init(void);
void USART3_IRQHandler(void);
void send_data(void);
void parse_recv_data(void);

#endif /* USART_H_ */
