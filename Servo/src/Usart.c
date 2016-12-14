/*
 * vrs_cv5.c
 *
 *  Created on: Oct 17, 2016
 *      Author: Jaroslav Duraj
 */

#include <Usart.h>
#include "stm32l1xx.h"
#include "stdio.h"
#include "main.h"

int i = 0;
int j = 0;
char Buffer_Data_to_send[7] = "";

uint16_t received_data[5];


uint16_t recv_speed = 0;
uint16_t recv_angle = 0;
uint16_t recv_stepping = 0;
uint16_t recv_start_angle = 0;
uint16_t recv_end_angle = 0;
uint16_t recv_mode = 0;

extern uint8_t Auto;



void UART1_init(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */       //turning on the needed peripherals
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  //choosing peripherals for selected pins
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

   /* Configure USART Tx and Rx pins */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //usart configuration
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

   //configuring interrupts
  /* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //choosing which event should cause interrupt

  USART_ITConfig(USART1, USART_IT_RXNE | USART_IT_TC, ENABLE);
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}


void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		received_data[i] = USART_ReceiveData(USART1);
		if(i==4)
		{
			i=0;
			parse_recv_data();
			set_recv_data();
		}
		else
		{
			i++;
		}


	}

	j++;
	if(j >= 50000)
	{
		j=0;
		if((USART_GetFlagStatus(USART1, USART_FLAG_TC) != RESET) )
		{

			send_data();

			USART_ClearFlag(USART1, USART_FLAG_TC);
		}
	}

}

void send_data(void)
{
		USART_SendData(USART1, (uint8_t)123);

}

void parse_recv_data(void)
{
	if(received_data[0] == 1)//mód manual
	{
		recv_mode = 0;
		recv_speed = received_data[1];
		recv_angle = received_data[2];
		recv_stepping = received_data[4];
	}
	if(received_data[0] == 2)//mód auto
	{
		recv_mode = 1;
		recv_speed = received_data[1];
		recv_start_angle = received_data[2];
		recv_end_angle = received_data[3];
		recv_stepping = received_data[4];
	}
}
