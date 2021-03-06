/*
 * decagon5te.h
 *
 *  Created on: 15/10/2019
 *      Author: Jes�s
 */

#ifndef DECAGON_H_
#define DECAGON_H_

//**********************************************************************************************************************************************************
#include "driverlib.h"

//**********************************************************************************************************************************************************
// Excitation pin
#define EXCITATION_PIN 			{P8DIR |= BIT0; P8REN &= ~BIT0;}
#define EXCITATION_PIN_H     	P8OUT |= BIT0
#define EXCITATION_PIN_L     	P8OUT &= ~BIT0

//**********************************************************************************************************************************************************
// RXD on P1.7 (Timer TA0 CCR1 capture: CCI1A input)
#define UART_RX_IN			P1DIR &= ~BIT7
#define UART_RX    			P1SEL0 |= BIT7

//**********************************************************************************************************************************************************
// Conditions for 1200 Baud SW UART, ACLK = 32768Hz
#define UART_TBIT_DIV_2     (32768 / (1200 * 2))
#define UART_TBIT           (32768 / 1200)

//**********************************************************************************************************************************************************
// Function prototypes
void Decagon_Init();
void Decagon_Start();
void Decagon_Stop();
void Decagon_Get_Data(uint8_t *);

#endif /* DECAGON_H_ */
