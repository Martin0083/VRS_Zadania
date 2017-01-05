/*
 * Spi.c
 *
 *  Created on: 14. 12. 2016
 *      Author: Jaroslav Duraj
 */
#include "Spi.h"
#include "stdio.h"
#include "main.h"


void spi_set_current(uint8_t current) //funkcia na nastavenie pr�dov�ho odberu na drive cez SPI.
{
	EnableDisable(0);// Disable
	WriteSPI1(0x09, current);//adresa,data, Nastavenie prudu Tval:5x31.25mA
	EnableDisable(1);// Enable
}

void spi_set_step_mode(uint8_t step_mode) //funkcia na nastavenie krokovania na drive cez SPI
{
	EnableDisable(0);// Disable
	WriteSPI1(0x16, 0b10001000 | (step_mode & 0b111));//Step mode el pos: 7(000), step mode: full(000)
	EnableDisable(1);// Enable
}

uint8_t  WriteSPI1(uint8_t  txAddr, uint8_t  txData) //funkcia na z�pis d�t na SPI zbernicu
{
 uint8_t rxData;

 device_Select();

 SPI1->DR = txAddr;
 // wait until TXE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
 // wait until RXNE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
 // read the rx buff to clear the RXNE flag (garbage)
 rxData = SPI1->DR;

 device_Unselect();

 device_Select();

 SPI1->DR = txData;
 // wait until TXE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
 // wait until RXNE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
 // read the rx buff to clear the RXNE flag (garbage)
 rxData = SPI1->DR;

 device_Unselect();

 return rxData;
}

uint8_t ReadSPI1(uint8_t  txAddr) //funkcia na ��tanie d�t na SPI zbernici
{
 uint8_t   rxData;

 device_Select();

 SPI1->DR = txAddr|0b00100000;
 // wait until TXE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
 // wait until RXNE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
 // read the rx buff to clear the RXNE flag (garbage)
 rxData = SPI1->DR;

 device_Unselect();

 device_Select();

 SPI1->DR = 0b00000000;
 // wait until TXE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
 // wait until RXNE = 1
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
 // read the rx buff to clear the RXNE flag (garbage)
 rxData = SPI1->DR;

 device_Unselect();

 return rxData;
}

uint8_t EnableDisable(int state) //funkcia zap�na a vyp�na most�k aby bolo mo�n� vykona� nastavenie parametrov motora
{

	uint16_t  rxData;
	uint16_t  txData;

	if(state == 1){
		txData = 0b10111000;//enable
	}else{
		txData = 0b10101000;//disable
	}

	device_Select();

	SPI1->DR = txData;
	//wait until TXE = 1;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
	//wait until RXNE = 1
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
	//read the rx buff to clear the RXNE flag (garbage)
	rxData = SPI1->DR;

	device_Unselect();

	return rxData;
}


void device_Select(void) //funkcia na resetnutie pinu SPI1_CS
{
	GPIOB->BSRRH = GPIO_Pin_6;
}

void device_Unselect(void) //funkcia na setnutie pinu SPI1_CS
{
	GPIOB->BSRRL = GPIO_Pin_6;
}

void initSPI1(void)//funkcia na inicializ�ciu SPI1 a SPI pinov PA5, PA6, PA7.
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	//PA5 - SPI1_SCK
	//PA6 - SPI1_MISO
	//PA7 - SPI1_MOSI
	//PA4 - SPI1_NSS (SPI1_CS)

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = SPI_CRC_Rx;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;


	/* Apply SPI configuration after enabling it */
	SPI_Init(SPI1, &SPI_InitStructure);
	/* SPI Peripheral Enable */
	SPI_Cmd(SPI1, ENABLE);
}

void initCS_Pin(void) //funkcia na inicializ�ciu pinu SPI1_CS - PA4
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

