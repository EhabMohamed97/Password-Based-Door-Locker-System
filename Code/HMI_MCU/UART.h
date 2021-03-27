/*
 * Module:USART.h
 *  Created on:Nov 18, 2020
 *   Author:Ehab
 *    MicroController Abstraction Layer(MCUAL)
 */

#ifndef UART_H_
#define UART_H_

/*--------------------------------- >Common files<-------------------------------------- */
#include"std_types.h"
#include"micro_config.h"
#include"common_macros.h"

/*------------------------------------>Definitions<----------------------------------*/
#define NULL_PTR (void*)0

/*----------------------------->Structures definitions<----------------------------------*/

/* Enumeration to define the character size */
typedef enum
{
	FIVE_BITS, SIX_BITS, SEVEN_BITS, EIGHT_BITS, NINE_BITS=7
}CharacterSize;

/* Enumeration to define the parity type */
typedef enum
{
	NO_PARITY, EVEN=2, ODD
}Parity;

/* Enumeration to define the number of stop bits */
typedef enum
{
	ONE, TWO
}StopBits;

/* UART configuration structure */
typedef struct
{
	CharacterSize DataSize;
	Parity ParityType;
	StopBits NoStopBits;   /*Number of stop bits*/
	uint32 BaudRate;
}UART_config;

/*----------------------------->Public global variables<---------------------------*/
extern volatile uint8 UART_receiveBuffer;

/*----------------------------->Public functions prototypes<---------------------------*/
void UART_init(UART_config*);
void UART_sendByte(const uint8);
uint8 UART_receiveByte();
void UART_sendString(const uint8*);
void UART_receiveString(uint8*);
void UART_setCallBack(void(*a_Ptr)());

#endif /* UART_H_ */
