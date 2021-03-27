/*
 * Moduole:Motors.h
 *  Created on:Nov 6, 2020
 *   Author: Ehab
 *    Motors driver
 *     HAL
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

/*common files:*/
#include"std_types.h"
#include"micro_config.h"
#include"common_macros.h"
#include "Timer.h"

/*----------------------------------------->DC motor connections<------------------------------------------*/
#define DC_MOTOR_PORT_DIRECTION DDRC
#define DC_MOTOR_PORT PORTC
#define L293D_IN1 PC2
#define L293D_IN2 PC3
#define L293D_EN1 PC4
#define CLOCKWISE 1
#define ANTI_CLOCKWISE 0

/* --->Functions prototypes: */
void DCMotor_init();
void DCMotor_direction(uint8);
void DCMotor_toggleDirection();
void DCMotor_speed(uint8 MotorSpeed);
void DCMotor_stop();

#endif /* DCMOTOR_H_ */
