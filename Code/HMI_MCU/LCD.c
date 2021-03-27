/*
 * Module:LCD.c
 *  Created on:Nov 2, 2020
 *   Author:Ehab
 *    LCD Driver
 *     Hardware Abstraction Layer (HAL)
 */

#include"LCD.h"

/*LCD initialization:*/
void LCD_init()
{
	LCD_CONTROL_PORT_DIRECTION |= (1<<RS) | (1<<RW) |(1<<E);   /*RS,RW and E are outputs*/

    #if(DATA_BITS_MODE == 4)

	   #ifdef UPPER_PINS
	    LCD_DATA_PORT_DIRECTION |= 0xF0;   /* The highest 4 pins is outputs */
		#else
	     LCD_DATA_PORT_DIRECTION |= 0x0F;   /* The lowest 4 pins is outputs */
	   #endif

	   LCD_sendCommand(TWO_LINES_FOUR_BITS_MODE);

	#elif(DATA_BITS_MODE == 8)
		 LCD_DATA_PORT_DIRECTION = 0xFF;   /* The data Port is output */
		 LCD_sendCommand(TWO_LINES_EIGHT_BITS_MODE);
	#endif

	LCD_sendCommand(LCD_CLEAR );   /*Clear display*/
	LCD_sendCommand(LCD_CURSOR_OFF);
 }   /*End LCD_init function*/

/*A function to send LCD commands:*/
void LCD_sendCommand(uint8 command)
{
	CLEAR_BIT(LCD_CONTROL_PORT,RS);   /*RS=0 to select command register*/
	CLEAR_BIT(LCD_CONTROL_PORT,RW);   /*RW=0 to write on data path*/
	_delay_us(1);                     /*tas=50ns from LCD timing diagram*/
	SET_BIT(LCD_CONTROL_PORT,E);      /*E=1 to enable LCD*/
	_delay_us(1);                     /*tpw-tdsw=190ns*/

    #if(DATA_BITS_MODE == 4)

	 #ifdef UPPER_PINS
	  /* Sending the highest 4 bits of command to the upper pins */
	  LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (command & 0xF0);
     #else
	  /* Sending the highest 4 bits of command to the lower pins */
	  LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | ((command & 0xF0) >> 4);
     #endif

	_delay_us(1);                 /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CONTROL_PORT,E);   /* disable LCD E=0 */
	_delay_us(1);                 /* delay for processing Th = 13ns */
	SET_BIT(LCD_CONTROL_PORT,E);     /*E=1 to enable LCD*/
	_delay_us(1);                 /*tpw-tdsw=190ns*/

	 #ifdef UPPER_PINS
	  /* Sending the lowest 4 bits of command to the upper pins */
	  LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((command & 0x0F) << 4);
     #else
	  /* Sending the lowest 4 bits of command to the lower pins */
	  LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (command & 0x0F);
     #endif

	_delay_us(1);                 /*tdsw=100ns*/
	CLEAR_BIT(LCD_CONTROL_PORT,E);   /* disable LCD E=0 */
	_delay_us(1);                 /*tah=13ns*/
   #elif(DATA_BITS_MODE == 8)
	LCD_DATA_PORT = command;      /*Send command on data path*/
	_delay_us(1);                 /*tdsw=100ns*/
	CLEAR_BIT(LCD_CONTROL_PORT,E);
	_delay_us(1);   /*tah=13ns*/
   #endif
 }   /*End LCD_SendCommand function*/

/*A function to display character on LCD:*/
void LCD_displayCharacter(uint8 chr)
{
	SET_BIT(LCD_CONTROL_PORT,RS);   /*RS=0 to select command register*/
	CLEAR_BIT(LCD_CONTROL_PORT,RW);   /*RW=0 to write on data path*/
	_delay_us(1);                     /*tas=50ns from LCD timing diagram*/
	SET_BIT(LCD_CONTROL_PORT,E);
	_delay_us(1);                     /*tpw-tdsw=190ns*/
	LCD_DATA_PORT = chr;
	_delay_us(1);                     /*tdsw=100ns*/
	CLEAR_BIT(LCD_CONTROL_PORT,E);
	_delay_us(1);                     /*tah=13ns*/
 }   /*End LCD_displayCharacter function*/

/*A function to display string on LCD:*/
void LCD_displayString(const char*string)
{
	uint8 i;

	for(i = 0; string[i] != '\0'; i++)LCD_displayCharacter(string[i]);

 }   /*End LCD_displayString function*/

/*A function to move the cursor to a specific location on LCD*/
void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 address;

	/*Get the desired address on LCD DD RAM*/
	switch(row)
	{
	   case 1:
		   address = col - 0x01;
		   break;
	   case 2:
		   address = col + 0x3f;
		   break;
	 }   /*End switch(row)*/

	LCD_sendCommand(SET_CURSOR_LOCATION | address);
 }   /*End LCD_goToRowCloumn function*/

/*A function to display string in a specific location on LCD*/
void LCD_displayStringRowColumn(uint8 row,uint8 col ,const char* str)
{
	LCD_goToRowColumn(row,col);
	LCD_displayString(str);
 }   /*End LCD_displayStringRowCloumn function*/

/*A function to clear LCD screen:*/
void LCD_clearScreen()
{
	LCD_sendCommand(LCD_CLEAR);
 }   /*End LCD_clearScreen function*/

/*A function to display integer on LCD:*/
void LCD_intgerToString(int num)
{
	char buff[20];
	itoa(num,buff,10);
	LCD_displayString(buff);
 }   /*End LCD_integerToString function*/
