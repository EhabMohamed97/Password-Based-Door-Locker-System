/*
 * Main.c
 *  Created on:Dec 7, 2020
 *   Author:EhabMohamed
 *    Control MicroController
 */

/*----------------------------------->Files<------------------------------------------*/
#include<string.h>
#include"EX_EEPROM.h"
#include"UART.h"
#include"DCMotor.h"
#include"Timer.h"
#include"Buzzer.h"

/*----------------------------------->Enumerations<------------------------------------------*/
typedef enum
{
	HMI_MCU_READY, NEW_PW, DONE_ONCE, OPEN_DOOR, CHANGE_PW, PW,CORRECT_PW,WRONG_PW,BUZZ_ON,OLD_PW,CORRECT_OLD_PW,
	WRONG_OLD_PW
}UART_messages;

/*---------------------------------------->Global variables<------------------------------------*/
/*
 * g_Control is a global control flag
 * g_counter is a global counter for timer
 * They're volatile because their values changes in The ISR
 * NewPassword is a global array to store the new password
 * Password is a global array to store any password entered by the user
 * ComparePW is a global array to store the PW stored in EEPROM
 */
volatile uint8 g_Control = 0, g_counter = 0;
uint8 NewPassword[7], Password[7], ComparePW[7];
TimerConfig s_timer1Config = {TIMER1, COMPARE, 0, F_CPU_1024, 7812};   /* Timer configuration structure */

/*---------------------------------------->Functions Prototypes<------------------------------------*/
void App_receiveControl();
bool App_isMatched(uint8*);
void App_timer1();
void App_timerDelay(uint8);

int main()
{
	/*
	 * OnceFlag is used to store the value of address 0 in EEPROM
	 * i and j is used as loop counter to handle EEPROM read and write operations
	 * readFlag is used to read the EEPROM read once for optimization
	 */
	uint8 OnceFlag, i = 0, j = 0, readFlag = 0;
	UART_config s_UARTConfig = {EIGHT_BITS, NO_PARITY, ONE, 9600};   /* UART configuration structure */

	SET_BIT(SREG, 7);   /* Enable The global interrupt */

	UART_init(&s_UARTConfig);               /* initialize the UART module with the configuration structure */
	UART_setCallBack(App_receiveControl);   /* Set the call back function to the App_receiveControl()function */

	EEPROM_init();                   /* initialize EEPROM module */
	EEPROM_readByte(0, &OnceFlag);   /* Read the value of address 0*/

	DCMotor_init();   /* Initialize the motor module */

	Timer_setCallBack(TIMER1,App_timer1);   /* Set the call back function to the App_timer1()function */

	Buzzer_init;   /* Initialize the buzzer module */

	while(1)
	{

		while(0 == g_Control);   /* Wait to receive a message from HMI_MCU */

		/*
		 * If"HMI_MCU_READY"received reset the global control flag
		 * Send the value of OnceFlag to HMI_MCU which contains the value of address 0 in EEPROM
		 */
		if(1 == g_Control)
		{
			g_Control = 0;
			UART_sendByte(OnceFlag);
		}   /* End if(1==g_Control) */
		/*
		 * If"NEW_PW" is received reset the global control flag
		 * Store the new password in EEPROM
		 */
		else if(2 == g_Control)
		{
			g_Control = 0;
			i = 0;

			while(NewPassword[i] != '\0')
			{
				 EEPROM_writeByte(i+1, NewPassword[i]);
				 _delay_ms(11);
				 i++;
			}   /* End While(NewPassword[i]!='\0') */

		}   /* End else if(2==g_Control) */
		else if(3 == g_Control)   /* If"DONE_ONCE"is received write 0x03 in address 0 in EEPROM */
		{
			g_Control = 0;
			EEPROM_writeByte(0, DONE_ONCE);
		}   /* End if(3==g_Control) */
		else if(4 == g_Control)   /* The user choose to open the door */
		{
			g_Control = 0;        /* Reset the global control flag */

			DCMotor_direction(CLOCKWISE);   /* Rotate motor clock wise for 15 seconds */
			App_timerDelay(15);
			DCMotor_stop();   /* Hold the motor for 2 seconds */

			App_timerDelay(2);

			DCMotor_direction(ANTI_CLOCKWISE);   /* Rotate the motor anti-clockwise for 15seconds */
			App_timerDelay(15);
			DCMotor_stop();   /* Stop the motor */

		}   /* End else if(4==g_Control) */
		else if(5 == g_Control)   /* If"OLD_PW"message is received */
		{
			g_Control = 0;

			if(App_isMatched(Password))UART_sendByte(CORRECT_OLD_PW);   /* Compare the old PW with the one in EEPROM */
			else UART_sendByte(WRONG_OLD_PW);                   /* If it's matched then send correct else send wrong */

		}   /* End else if(5==g_Control) */
		else if(6 == g_Control)   /* If any password is received */
		{
			g_Control = 0;

			if(0 == readFlag)   /* If read flag is zero this means that we didn't read the PW before */
			{

				for(j = 1; j <= 5; j++)
				{
					EEPROM_readByte(j, &ComparePW[j-1]);   /* Read the password stored in EEPROM */
					_delay_ms(12);
				}

				readFlag = 1;   /* Set read flag in order not to read EEPROM again for optimization */
			}   /* End if(0==readFlag) */

			if(App_isMatched(Password))UART_sendByte(CORRECT_PW);   /* Compare the received PW with the one stored in */
			else UART_sendByte(WRONG_PW);                           /*  EEPROM */

		}   /* End else if(6==g_Control) */
		else if(7 == g_Control)   /* If"BUZZ_ON"is received */
		{
			g_Control = 0;
			Buzzer_on;   /* Turn on the buzzer for 1 minute */
			App_timerDelay(60);
			Buzzer_off;
		}   /* End else if(7==g_Control) */

	}   /* End while(1) */

	return 0;
}   /* End main() function */

/* A function to control the UART receive messages */
void App_receiveControl()
{

	if(HMI_MCU_READY == UART_receiveBuffer)g_Control = 1;
	else if(NEW_PW == UART_receiveBuffer)
	{
		g_Control = 2;
		UART_receiveString(NewPassword);
	}   /* End else if(NEW_PW==UART_receiveBuffer) */
	else if(DONE_ONCE == UART_receiveBuffer)g_Control = 3;
	else if(OPEN_DOOR == UART_receiveBuffer)g_Control = 4;
	else if(PW == UART_receiveBuffer)
	{
		g_Control = 6;
		UART_receiveString(Password);   /*Receive the password to compare it with the correct password */
	}   /* End else if(PW==UART_receiveBuffer) */
	else if(BUZZ_ON == UART_receiveBuffer)g_Control = 7;
	else if(OLD_PW == UART_receiveBuffer)
	{
		g_Control = 5;
		UART_receiveString(Password);
	}   /* End else if(OLD_PW==UART_receiveBuffer) */

}   /* End App_receiveControl */

/* A function to compare the password with th one store in EEPROM */
bool App_isMatched(uint8*a_first)
{
	if(0 == (strcmp((char*)a_first, (char*)ComparePW)))return TRUE;
	else return FALSE;
}   /* End App_isMatched */

/* A function to set the time for the motor or any other module using timer1 */
void App_timer1()
{
	g_counter++;   /* Increment a global counter every compare match interrupt */

}   /* End App_timer1()function */

void App_timerDelay(uint8 delay)
{

	if(2 == delay)
	{
		Timer_init(&s_timer1Config);   /* initialize the timer module with the desired configuration */
		while(g_counter != 2);         /* wait until the global counter became 2 to make 2 seconds delay */
		g_counter = 0;                 /* Reset the global counter */
		Timer_stop(TIMER1);            /* Stop the timer */
	}   /* End if(2==delay) */
	else if(15 == delay)
	{
		Timer_init(&s_timer1Config);
		while(g_counter != 15);   /* wait until the global counter became 15 to make 15 seconds delay */
		g_counter = 0;
		Timer_stop(TIMER1);
	}   /* End else if(15==delay) */
	else if(60 == delay)
	{
		Timer_init(&s_timer1Config);
		while(g_counter != 60);   /* wait until the global counter became 60 to make 1 minute delay */
		g_counter = 0;
		Timer_stop(TIMER1);
	}   /* End else if(60==delay) */

}   /* End App_timerDelay()function */
