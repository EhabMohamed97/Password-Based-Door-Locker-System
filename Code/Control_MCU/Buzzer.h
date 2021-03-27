/*
 * Module:Buzzer.h
 *  Created on:Dec 16, 2020
 *   Author:Ehab
 *    Hardware Abstraction Layer (HAL)
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/*--------------------------------- >Common files<-------------------------------------- */
#include"std_types.h"
#include"micro_config.h"
#include"common_macros.h"

/*------------------------------------>Buzzer connections<-------------------------------*/
#define BUZZ_PORT_DIR DDRC
#define BUZZ_PORT PORTC
#define BUZZ PC5

/*------------------------------------>Buzzer functions as macros<-------------------------------*/
#define Buzzer_init SET_BIT(BUZZ_PORT_DIR ,BUZZ)   /* To make the buzzer pin output */
#define Buzzer_on SET_BIT(BUZZ_PORT, BUZZ)         /* To turn the buzzer on */
#define Buzzer_off CLEAR_BIT(BUZZ_PORT, BUZZ)      /* To turn the buzzer off */

#endif /* BUZZER_H_ */
