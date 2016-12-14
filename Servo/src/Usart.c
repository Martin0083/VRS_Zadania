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

uint16_t received_data[7];


uint16_t recv_speed = 0;
float recv_angle = 0;
uint16_t recv_stepping = 0;
float recv_start_angle = 0;
float recv_end_angle = 0;
uint16_t recv_mode = 0;

extern uint8_t Auto;



void UART3_init(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */       //turning on the needed peripherals
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  //choosing peripherals for selected pins
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

   /* Configure USART Tx and Rx pins */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //usart configuration
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);

   //configuring interrupts
  /* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //choosing which event should cause interrupt

  USART_ITConfig(USART3, USART_IT_RXNE | USART_IT_TC, ENABLE);
  /* Enable USART */
  USART_Cmd(USART3, ENABLE);
}


void USART3_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET)
	{
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		received_data[i] = USART_ReceiveData(USART3);
		if(i==6)
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
		if((USART_GetFlagStatus(USART3, USART_FLAG_TC) != RESET) )
		{

			send_data();

			USART_ClearFlag(USART3, USART_FLAG_TC);
		}
	}

}

void send_data(void)
{
		USART_SendData(USART3, (uint16_t)12345);

}

void parse_recv_data(void)
{
	if(received_data[0] == 1)// manual mód
	{
		recv_mode = 0;
		recv_speed = received_data[1];
		recv_angle = (float)(received_data[2]+ ((received_data[3]<<8)&0xFF00))/100;
		recv_stepping = received_data[4];

	}
	if(received_data[0] == 2)//auto mód
	{
		recv_mode = 1;
		recv_speed = received_data[1];
		recv_start_angle = (float)(received_data[2]+ ((received_data[3]<<8)&0xFF00))/100;
		recv_stepping = received_data[4];
		recv_end_angle = (float)(received_data[5]+ ((received_data[6]<<8)&0xFF00))/100;
	}
}
