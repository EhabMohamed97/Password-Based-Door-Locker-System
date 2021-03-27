/*
 * HMI_MCU.c
 *  Human Machine Interface MicroController
 *   Created on:Dec 6, 2020
 *    Author:EhabMohamed
 */

/*--------------------------------------->Files<------------------------------------------*/
#include<string.h>
#include"LCD.h"
#include"UART.h"
#include"Keybad.h"
#include"Timer.h"

/*--------------------------------------->Enumerations<------------------------------------------*/
typedef enum
{
	HMI_MCU_READY, NEW_PW, DONE_ONCE, OPEN_DOOR, CHANGE_PW, PW,CORRECT_PW,WRONG_PW,BUZZ_ON,OLD_PW,CORRECT_OLD_PW,
	WRONG_OLD_PW
}UART_messages;

/*----------------------------------->Functions prototypes<------------------------------------------*/
void App_newPassword();
void App_getPassword(uint8*);
void App_mainOptions();
void App_receiveControl();
void App_timer1();
void App_timerDelay(uint8);

/*----------------------------------->Global Variables<------------------------------------------*/
/*
 * OnceFlag is a flag used to make App_newPassword() function execute once in lifetime
 * NewPassword[7] is a global array to hold the new 5 digits password
 * Password[7] is a global array to hold the 5 digits user entered password
 * againPassword0[7] is a global array to hold the reentered new password
 */
uint8 OnceFlag = 0, NewPassword[7], Password[7], againPassword0[7];

/*
 * g_Control is a global control flag used to control the application code
 * wrongTry is a global flag to count the wrong passwords
 * Note:They're volatile variables because there values are changing by ISR
 */
volatile uint8 g_Control = 0, wrongTry = 0, g_counter = 0;
TimerConfig s_timer1Config = {TIMER1, COMPARE, 0, F_CPU_1024, 7812};   /* Timer1 configuration structure */

int main()
{
	/*
	 * result is a variable to hold the result of comparing the new password and the reentered one
	 * s_UARTConfig UART configuration structure
	 */
	UART_config s_UARTConfig = {EIGHT_BITS, NO_PARITY, ONE, 9600};

	UART_init(&s_UARTConfig);        /* Initialize UART with the specified frame in the UART configuration structure */
	UART_sendByte(HMI_MCU_READY);    /* Send a ready message to Control_MUC to receive the EEPROM flag */
	OnceFlag = UART_receiveByte();   /* Receive the EEPROM flag */

	LCD_init();
	LCD_displayStringRowColumn(1,5,"Welcome");
	_delay_ms(500);
	LCD_clearScreen();

	SET_BIT(SREG, 7);   /* Enable The global interrupt */

	Timer1_setCallBack(App_timer1);   /* Set timer1 call back to App_timer1()function to handle the ISR */

	UART_setCallBack(App_receiveControl);   /* Set UART call back to App_receiveControl()function to handle the ISR */

	while(1)
	{

		/*
		 * Once flag holds the value of address 0 in EEPROM which is 0xFF in the beginning
		 * Execute App_newPassword() function to create a new password in the first time in lifetime
		 * OnceFlag=0x02 and send this byte to Control_MCU to make the value of address0 in EEPROM 0x02
		 * In the end of the first run of the program the flag became 0x03 so this code will run once in lifetime
		 */
		if(0xFF == OnceFlag)
		{
			App_newPassword();
			OnceFlag = DONE_ONCE;
			UART_sendByte(OnceFlag);
		}   /* End if(0xFF== OnceFlag) */
		/*
		 * g_Control=0 in the first of every run of the program to make the user enter the password
		 * Store the password in Password array
		 * Send PW to Control_MCU to make it ready to receive the stored password
		 * Wait the reply of Control_MCU which is correct or wrong password
		 */
		if(0 == g_Control)
		{
			LCD_displayStringRowColumn(1, 1, " Enter The Pass");
			LCD_displayStringRowColumn(2, 1, "              ");
			App_getPassword(Password);
			UART_sendByte(PW);
			UART_sendString(Password);
			g_Control = 1;
			while(1 == g_Control);
		}   /* End if(0==g_Control) */
		/*
		 * If the global flag became 2 it means that the entered password is correct
		 * Then go to App_mainOptions() function to open the door or change the password
		 */
		else if(2 == g_Control)
		{
			g_Control = 2;
			App_mainOptions();
		}   /* End else if(2==g_Control) */
		/*
		 * If the first entered password is wrong
		 * Ask the user to enter the password again
		 * Every time he entered the PW send this PW to Control_MCU to check if this PW is correct or wrong
		 * Wait for the reply of Control_MCU
		 */
		else if(3 == g_Control)
		{
			g_Control = 1;

			LCD_clearScreen();
			LCD_displayStringRowColumn(1, 5,"Wrong");
			_delay_ms(500);
			LCD_displayStringRowColumn(1, 1, "Enter Pass again");
			App_getPassword(Password);
			UART_sendByte(PW);
			UART_sendString(Password);
			while(0 == g_Control);
		}   /* End else if(3==g_Control) */
		/*
		 * If the user Entered the wrong PW 3 times
		 * Send BUZ_ON message to Control_MCU to turn on the buzzer
		 * then display"Error"message on LCD for 1 minute while the buzzer is running
		 * Then set the global flag to 0 to enter the PW
		 */
		else if(4 == g_Control)
		{
			UART_sendByte(BUZZ_ON);
			LCD_clearScreen();
			LCD_displayStringRowColumn(1,5, "Error");
			App_timerDelay(60);
			g_Control = 0;
		}   /* End else if(4==g_Control) */
		/*
		 * If the user chose to change the PW and entered the old PW correctly
		 * Execute App_newPassword()function to allow user to create a new PW
		 * Set the global control flag to 2
		 */
		else if(5 == g_Control)
		{
			App_newPassword();
			g_Control = 2;
		}/* End else if(5==g_Control) */
		/*
		 * If the user entered the wrong old PW
		 * Display Wrong for 0.5second then ask him to enter the correct old PW
		 * Store the entered PW in Password array then send OLD_PW message to Control_MCU to make it ready to receive
		 *  the entered password
		 * Wait the reply of Control_MCU
		 */
		else if(6 == g_Control)
		{
			g_Control = 1;
			LCD_clearScreen();
			LCD_displayStringRowColumn(1, 5,"Wrong");
			_delay_ms(500);
			LCD_clearScreen();
			LCD_displayStringRowColumn(1,1,"Enter The old PW");
			App_getPassword(Password);
			UART_sendByte(OLD_PW);
			UART_sendString(Password);
			while(1 == g_Control);
		}

	}   /* End while((1) */

	return 0;
}   /* End main()function */

/*----------------------------------->Functions definitions<------------------------------------------*/
/* A function to make new password */
void App_newPassword()
{
	/*
	 * Ask the user to enter the new PW and store it in NewPassword array
	 * Then ask him to reenter it until it became a valid PW
	 * Send NEW_PW message to Control_MCU to store it in EEPROM
	 * Then tell the user that the new PW is confirmed
	 */
	uint8 result;

	LCD_displayStringRowColumn(1,1,"Enter new Pass  ");
	LCD_displayStringRowColumn(2,1,"                ");   /* Clear the second row to enter password */
	App_getPassword(NewPassword);

	do
	{
		LCD_displayStringRowColumn(1,1,"ReEnter Password");
		LCD_displayStringRowColumn(2,1,"                ");
		App_getPassword(againPassword0);
		result = strcmp((char*)NewPassword, (char*)againPassword0);

		if(0 != result)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(1, 5,"Wrong");
			_delay_ms(500);
		}
	}while(0 != result);

	UART_sendByte(NEW_PW);
	UART_sendString(againPassword0);
	LCD_clearScreen();
	LCD_displayStringRowColumn(1, 4, "Confirmed");   /* The second password is matched with the new one */
	_delay_ms(500);
	LCD_clearScreen();
}   /* End App_newPassword()function */

/* A function to take the password from the keypad and store it */
void App_getPassword(uint8*a_Password)
{
	sint8 i;
	uint8 key;

	for(i = 0; i < 5; i++)
	{
		key = Keypad_getPressedKey();

		if(key >= 0 && key <= 9)
		{
			a_Password[i] = key;

			LCD_displayStringRowColumn(2, i+6, "*");   /* Display'*'to hide the password for security */
			_delay_ms(500);                             /* Delay between keys which means accepting key every 0.5S */
		}   /* End if(key >= 0 && key <= 9) */
		else i--;   /* To Loop until the user enter a password */

		if(4 == i)
		{
			key = Keypad_getPressedKey();

			if(13 == key)break;               /* The user entered a 5 digits password */

		}   /* End if(4==i) */

	}   /* End for(i = 0; i< 5; i++) */

	a_Password[5] = '#';
	a_Password[6] = '\0';

}   /* End App_getPassword()function */

/* A function to handle the application main options */
void App_mainOptions()
{
	/*
	 * Ask the user to enter'+'to open the door or'-'to change the PW
	 *
	 * If he chose to open the door so send OPEN_DOOR message to Control_MCU to rotate the DC motor
	 * Display"Door's unlocking"message in LCD while the motor is rotating clockwise
	 * Delay 2 seconds without any message on LCD using timer1 through App_timerDelay() function
	 * Display"Door's locking"message in LCD while the motor is rotating anti-clockwise
	 *
	 * If he chose to change the password so ask him to enter the old password and store it in password array
	 * Send OLD_PW message to Control_MCU to check if this old PW is correct or wrong
	 * Wait the replay of Control_MCU which is either CORRECT_OLD_PW or WRONG_OLD_PW
	 */
	LCD_displayStringRowColumn(1, 1, "+ : Open Door   ");
	LCD_displayStringRowColumn(2, 1, "- : Change Pass");

	while(1)
	{

		if('+' == Keypad_getPressedKey())
		{
			UART_sendByte(OPEN_DOOR);
			LCD_clearScreen();

			LCD_displayStringRowColumn(1, 1, "Door's unlocking");
			App_timerDelay(15);   /* Delay 15 seconds using timer1 */
			LCD_clearScreen();

			App_timerDelay(2);    /*Delay 2 second suing timer1 */

			LCD_displayStringRowColumn(1, 1, "Door's locking   ");
			App_timerDelay(15);   /* Delay 15 seconds again using timer1 */
			LCD_clearScreen();

			break;   /* To break from the inner while(1) to execute the following application code */
		}   /* End if('+'==Keypad_getPressedKey()) */
		else if('-' == Keypad_getPressedKey())
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(1,1,"Enter The old PW");

			App_getPassword(Password);
			UART_sendByte(OLD_PW);
			UART_sendString(Password);

			while(2 == g_Control);

			break;   /* To break from the inner while(1) to execute the following application code */
		}   /* End if('-'==Keypad_getPressedKey()) */
	}   /* End while(1) */

}   /* End App_mainOptions()function */

/* A function to control the UART receive messages */
void App_receiveControl()
{
	/*
	 * If COORECT_PW is received set the global control flag to 2 and clear the wrongTry flag
	 * If WRONG_PW is received set the global flag to 3 and increment the wrongTry flag to count the number
	 *  of wrong try if it reached 3 then set g_Control to 4 to make its actions
	 * If CORRECT_OLD_PW is received then make g_Control=5 to allow the user to create an new PW
	 * IF WRONG_OLD_PW is received then make g_Control=6 to make the user enter it the old PW again
	 *  ,if he entered it 3 times wrong make g_Control=4 to make its actions
	 */
	if(CORRECT_PW == UART_receiveBuffer)
	{
		g_Control = 2;
		wrongTry = 0;
	}   /* End if(CORRECT_PW==UART_receiveBuffer) */
	else if(WRONG_PW == UART_receiveBuffer)
	{
		g_Control = 3;
		wrongTry++;

		if(3 == wrongTry)
		{
			wrongTry = 0;
			g_Control = 4;
		}   /* End if(3==wrongTry) */

	}   /* End else if(WRONG_PW==UART_receiveBuffer) */
	else if(CORRECT_OLD_PW == UART_receiveBuffer)g_Control = 5;
	else if(WRONG_OLD_PW == UART_receiveBuffer)
	{
		g_Control = 6;
		wrongTry++;

		if(3 == wrongTry)
		{
			wrongTry = 0;
			g_Control = 4;
		}   /* End if(3==wrongTry) */

	}   /* End else if(WRONG_OLD_PW==UART_receiveBuffer) */

}   /* End App_receiveControl()Function */

/* A function to control timer1 */
void App_timer1()
{
	g_counter++;   /* increment the global counter flag with every interrupt */
}   /* End App_timer1()function */

/* A function to make a time delay using timer1 */
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
