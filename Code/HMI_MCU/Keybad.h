/*
 * Module:Keypad.h
 *  Created on:Nov 11, 2020
 *   Author:Ehab
 *    Hardware Abstraction Layer (HAL)
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

/* Common header files: */
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/* Static configurations: */
#define N_ROW 4
#define N_COL 4

/* Keypad connections: */
#define KEYPAD_PORT_DIR DDRB
#define KEYPAD_PORT PORTB
#define KEYPAD_PIN PINB

/* Public functions prototypes: */
uint8 Keypad_getPressedKey();


#endif /* KEYPAD_H_ */
