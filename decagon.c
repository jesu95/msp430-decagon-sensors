/*
 * 5te.c
 *
 *  Created on: 15/10/2019
 *      Author: Jesús
 */
#include "decagon.h"

//**********************************************************************************************************************************************************
// Global variable used for UART communication reception
static uint8_t rxBuffer[20] = {0};                      // Received UART character

//**********************************************************************************************************************************************************
// Inicializa los pines y el timer para recibir los datos del sensor
void Decagon_Init()
{
	EXCITATION_PIN;
	EXCITATION_PIN_L;
	UART_RX_IN;
	UART_RX; 											// Timer function for RXD pin
}

//**********************************************************************************************************************************************************
// Excita el sensor para tomar una medida y que luego este envie los datos.
void Decagon_Start()
{
    TA0CTL = TACLR;
    TA0CCTL1 &= ~CCIFG;
    TA0CCTL1 |= SCS + CCIS_0 + CM_2 + CAP + CCIE;       // Sync, Neg Edge (Start Condition), Capture, Int
    TA0CTL |= TASSEL_1 + MC_2;                          // ACLK, Continuos Mode

	EXCITATION_PIN_L;
	EXCITATION_PIN_H;

    __no_operation();
    __bis_SR_register(LPM3_bits);
}

//**********************************************************************************************************************************************************
// Desenergiza el sensor porque para recibir los datos en DDi se debe energizar ciclicamente.
void Decagon_Stop()
{
	EXCITATION_PIN_L;
}

//**********************************************************************************************************************************************************
// Copia los datos de la variable global (pero local al archiva para encapsular) rxBuffer.
void Decagon_Get_Data(uint8_t *data)
{
	uint8_t i;

    for(i = 20 ; i > 0 ; i--)
    {
        data[20u - i] = rxBuffer[20u - i];
    }
}

//**********************************************************************************************************************************************************
// Timer_A UART - Receive Interrupt Handler
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
	static uint8_t rxBitCnt = 8;
    static uint8_t rxData = 0;
    static uint8_t i = 0;

    switch (__even_in_range(TA0IV, TA0IV_TAIFG)) {  // Use calculated branching
        case TA0IV_TACCR1:                          // TACCR1 CCIFG - UART RX
            TA0CCR1 += UART_TBIT;                   // Add Offset to CCRx
            if (TA0CCTL1 & CAP) {                   // Capture mode = start bit edge
                TA0CCTL1 &= ~CAP;                   // Switch capture to compare mode
                TA0CCR1 += UART_TBIT_DIV_2;         // Point CCRx to middle of D0
            }
            else {
            	rxData >>= 1;
                if (TA0CCTL1 & SCCI) {              // Get bit waiting in receive latch
                	rxData |= 0x80;
                }
                rxBitCnt--;
                if (rxBitCnt == 0) {                // All bits RXed?
                    rxBuffer[i] = rxData;           // Store in global variable
                    rxBitCnt = 8;                   // Re-load bit counter
                    TA0CCTL1 |= CAP;                // Switch compare to capture mode

                    if ((rxBuffer[i] == 0x0A || i == 12)){
                    	i = 0;
                    	EXCITATION_PIN_L;
                    	TA0CTL = 0;                 // Stop timer
                    	TA0CCTL1 = 0;
                        TA0CCR1 = 0;
                        __bic_SR_register_on_exit(LPM3_bits);
                    }else{
                    	i++;
                    }
                }
            }
            break;
    }
}
