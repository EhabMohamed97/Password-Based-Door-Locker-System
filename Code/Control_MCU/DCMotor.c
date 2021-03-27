/*
 * Module:Motors.c
 *  Created on:Nov 6, 2020
 *   Author:Ehab
 *    Direct Current Motors driver using L293D chip
 *     Harware Abstraction Layer (HAL)
 */

#include "DCMotor.h"

/*----------------------------------->Public Functions definitions<------------------------------- */
/* A function to initialize DC motor: */
void DCMotor_init()
{
	/* Motor is connected to L293D Driver
	 *  L293D is connected to PORTB
	 *   So PB0,PB1 and PB3 are outputs which
	 *    connected to L293D IN1,IN2 and EN1 respectively
	 */
	DC_MOTOR_PORT_DIRECTION = (1<<L293D_IN1) | (1<<L293D_IN2) |(1<<L293D_EN1);

 }   /* End DCMotor_init function */

/* A function to choose the direction of DC motor: */
void DCMotor_direction(uint8 direction)
{

	if(direction == CLOCKWISE)
	{
		CLEAR_BIT(DC_MOTOR_PORT,L293D_IN1);
		SET_BIT(DC_MOTOR_PORT,L293D_IN2);
	 }   /* End if(direction == CLOCKWISE) */
	else if(direction == ANTI_CLOCKWISE)
	{
		SET_BIT(DC_MOTOR_PORT,L293D_IN1);
		CLEAR_BIT(DC_MOTOR_PORT,L293D_IN2);
	 }   /*End else if(direction == ANTI_CLOCKWISE) */

	SET_BIT(DC_MOTOR_PORT, L293D_EN1);

 }   /* End DCMotor_direction */

/* A function to control the speed of a DC motor: */
void DCMotor_speed(uint8 MotorSpeed)
{
	Timer0_PWM_Init(MotorSpeed);        /* Control DC Motor Speed with timer0 PWM signal */

 }   /* End DCMotors_Speed function */

/* A function to toggle the direction of the motor */
void DCMotor_toggleDirection()
{
	TOGGLE_BIT(DC_MOTOR_PORT,L293D_IN1);
	TOGGLE_BIT(DC_MOTOR_PORT,L293D_IN2);
 } /* End DCMotor_toggleDirection() function */

/* A function to stop the motor */
void DCMotor_stop()
{
	CLEAR_BIT(DC_MOTOR_PORT, L293D_EN1);   /* Clear the enable pin in the L293D driver */
}   /* End DCMotor_stop()function */


