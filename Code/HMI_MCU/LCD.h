/*
 * Module: LCD.h
 *  Created on: Nov 2, 2020
 *   Author: Ehab
 *    LCD Driver
 *     Hardware Abstraction Layer (HAL)
 */

#ifndef LCD_H_
#define LCD_H_

#include"common_macros.h"
#include"std_types.h"
#include"micro_config.h"

/* LCD mode configurations: */
#define DATA_BITS_MODE 8

#if(DATA_BITS_MODE == 4)
 #define UPPER_PINS   /* D4--D7 is connected to PC4--PC7 */
#endif

/*LCD Connections:*/
#define LCD_CONTROL_PORT_DIRECTION DDRC
#define LCD_CONTROL_PORT PORTC
#define LCD_DATA_PORT_DIRECTION DDRA
#define LCD_DATA_PORT PORTA
#define RS PC0
#define RW PC1
#define E PC2

/*LCD commands:*/
#define LCD_CLEAR 0x01
#define TWO_LINES_EIGHT_BITS_MODE 0x38
#define TWO_LINES_FOUR_BITS_MODE 0x28
#define SET_CURSOR_LOCATION 0x80
#define LCD_CURSOR_OFF 0x0C

/*Functions prototypes:*/
 void LCD_init();
 void LCD_sendCommand(uint8);
 void LCD_displayCharacter(uint8);
 void LCD_displayString(const char*);
 void LCD_goToRowColumn(uint8,uint8);
 void LCD_displayStringRowColumn(uint8,uint8,const char*);
 void LCD_clearScreen();
 void LCD_intgerToString(int);

#endif   /* LCD_H_ */
