/*
 * Module:UART.c
 *  Created on:Nov 18, 2020
 *   Author:Ehab
 *    Universal Synchronous Asynchronous Receiver Transmitter driver
 *     Microcontroller Abstraction Layer (MCUAL)
 */

#include"UART.h"

/*---------------------------------->Public global variables<------------------------------*/
volatile uint8 UART_receiveBuffer;

/*---------------------------------->Private global variables<------------------------------*/
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*---------------------------------->Public functions definitions<------------------------------*/
/* A function to initialize UART module */
void UART_init(UART_config*config_Ptr)
{
	uint16 UART_preScale;   /* to hold the UBRR value */

	/*
	 * UART Control and Status Register A
	 * TXC=0         No transmission in the beginning
	 * UDRE=0        UDR is empty in the beginning
	 * FE,DOR,PE=0   To disable Frame Error,Data Overrun Error and Parity Error respectively
	 * U2X=1         To enable Asynchronous double speed mode
	 * MPCM=0        To disable Multi-Processor Communication Mode
	 */
	SET_BIT(UCSRA,U2X);

	/*
	 * UART Control and Status Register B
	 * First, I clear the last  5 bits then:
	 *  RXCIE=1       To enable RXC interrupt
	 *  TXCIE=0        To disable TXC interrupt
	 *  UDRIE=0        To disable UDRE interrupt
	 *  RXEN=1         To enable UART to receive data
	 *  TXEN=1         To enable UART to transmit data
	 *  If nine bits character size is selected in the configuration structure
	 *  then: UCSZ2=1
	 */
	UCSRB = (UCSRB & 0x67) | 0x98;
	if(config_Ptr->DataSize == NINE_BITS)SET_BIT(UCSRB,UCSZ2);

	/*
	 * UART Control and Status Register C
	 * URSEL=1                       To enable writing in UCSRC
	 * UMSEL=0                       To select Asynchronous Operation
	 * UPM1:0=a_config->ParityType   To Select the desired parity type
	 * USBS=a_config->NoStopBits     To select the desired number of stop bits
	 * UCSZ1:0=a_config->DataSize    To select the desired character size
	 */
	SET_BIT(UCSRC,URSEL);
	UCSRC = (UCSRC & 0xCF) | (config_Ptr->ParityType << 4);
	if(config_Ptr->NoStopBits == TWO)SET_BIT(UCSRC,USBS);
	UCSRC = (UCSRC & 0xF9) | ((config_Ptr->DataSize & 0x03)<<1);

	/* UART Baud Rate Register
	 * UBRR=config_Ptr->BaudRate   To Select the desired baud rate
	 */
	UART_preScale = (F_CPU /( 8 * (config_Ptr->BaudRate))) - 1;
	//CLEAR_BIT(UCSRC,URSEL);
	UBRRH = (UART_preScale >> 8);
	UBRRL = UART_preScale;

}   /* End UART_init()function */

/* A function to send byte */
void UART_sendByte(const uint8 data)
{
	while(BIT_IS_CLEAR(UCSRA,UDRE));   /* Wait until UDR became empty */

	/* Put the required data in UDR and this also clear the UDRE flag because UDR isn't empty now */
	UDR = data;
}   /* End UART_sendByte()function */

/* A function to receive byte */
uint8 UART_receiveByte()
{

	while(BIT_IS_CLEAR(UCSRA,RXC));   /* Wait until there's new unread data in UDR */
	return UDR;                       /* return this data */
}   /* End UART_recieveByte()function */

/* A function to send a string */
void UART_sendString(const uint8*string_Ptr)
{
	uint8 i = 0;

	while(string_Ptr[i] != '\0')
	{
		UART_sendByte(string_Ptr[i]);
		i++;
	}   /* End while(string_Ptr[i] != '\0') */

}   /* End UART_sendString(uint8*) function */

/* A function to receive a string */
void UART_receiveString(uint8*Str)
{
	uint8 i = 0;

	Str[i] = UART_receiveByte();   /* To receive the first character in the string */

	while(Str[i] != '#')           /* '#'is a unique symbol as the end of string indication */
	{
		i++;
		Str[i] = UART_receiveByte();
	}                             /* End while(Str[i] != '#') */

	Str[i] = '\0';                /* Replace'#' with the null character to make it a valid string */
}   /* End UART_receiveString()function */

void UART_setCallBack(void(*a_Ptr)())
{
		g_callBackPtr = a_Ptr;

}   /* End UART_setCallBack()function */

ISR(USART_RXC_vect)
{
	UART_receiveBuffer = UART_receiveByte();

	if(g_callBackPtr != NULL_PTR)(*g_callBackPtr)();

}   /* End ISR(USART_RXC_vect) */
