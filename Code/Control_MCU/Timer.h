/*
 * Module:Timers.h
 *  Created on:Nov 6, 2020
 *   Author:Ehab
 *    ATMega16 Timers driver
 *     MicroController Abstraction Layer (MCAL)
 */

#ifndef TIMER_H_
#define TIMER_H_

#include"std_types.h"
#include"micro_config.h"
#include"common_macros.h"

/*--------------------->Structures and enumerations definitions<-------------------------- */
#define NULL_PTR (void*)0

/* Enumeration to select the timer module */
typedef enum
{
	TIMER0,TIMER1,TIMER2
}TimerSelect;

/* Enumeration to select the timer mode */
typedef enum
{
	NORMAL,COMPARE
}TimerModeSelect;

/*Enumeration to select the timer clock */
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}TimerClockSelect;

/* Timer module Configuration structure */
typedef struct
{
	TimerSelect timer;        /* To select timer0 or timer1 or timer2 */
	TimerModeSelect mode;     /* To select the timer mode which is normal or compare */
	uint16 initialValue;      /* To initialize the timer */
	TimerClockSelect clock;   /* To select the timer prescaler */
	uint16 compareValue;      /* To initialize the compare register by compare value */
}TimerConfig;

/*---------------------------->Public functions prototypes<------------------------------*/
void Timer0_PWM_Init(uint8 DutyCycle);
void Timer_init(const TimerConfig*);
void Timer_stop(TimerSelect);
void Timer_clearValue(TimerSelect);
void Timer_setCallBack(TimerSelect timer,void(*a_Ptr)());

#endif /* TIMER_H_ */
