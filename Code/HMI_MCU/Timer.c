/*
 * Module:Timers.c
 *  Created on:Nov 6, 2020
 *   Author:Ehab
 *    ATMega16 Timers driver
 *     MicroController Abstraction Layer (MCAL)
 */

#include "Timer.h"

/*-------------------------->Private global variables<------------------------------*/
static volatile void(*g_callBackPtr)() = NULL_PTR;

/*-------------------------->Public Functions Definitions<------------------------------*/
/* A function to initialize Timer0 in PWM Mode: */
void Timer0_PWM_Init(uint8 DutyCycle)
{
	SET_BIT(DDRB,PB3);   /* OC0(PB3) is output to generate PWM Signal */

	/* Timer/Counter0 Control Register TCCR0
	 * WGM01:0=0b11 To select fast PWM mode
	 * COM01:0=0b10 To select PWM non-inverting mode
	 * CS02:0=0b010 To select prescaler=8
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);

	TCNT0 = 0;          /* Timer0 initial value=0 */
	OCR0 = DutyCycle;   /* Set OCR0 by the desired duty cycle */

 }   /* End Timer0_PWM_Init function */

/* A function to initialize all timers */
void Timer_init(const TimerConfig*config_Ptr)
{

	if(config_Ptr->timer == TIMER0 && config_Ptr->mode == NORMAL)
	{
		TCNT0 = 0;
		SET_BIT(TIMSK, TOIE0);
		TCCR0 = (1<<FOC0) | (1<<CS02) | (1<<CS00);
	}   /* End if(config_Ptr->timer==TIMER0) */
	else if(config_Ptr->timer == TIMER1)
	{

		if(config_Ptr->mode == NORMAL)
		{
			/*
			 * Timer/Counter1 Control Register1 A (TCCR1A)
			 * COM1A1:0/COM1B1:0=0b0000   To select timer1 normal mode operation
			 * FOC1A/FOC1B=0b11           Non-PWM mode (Normal mode)
			 * WGM11:10=0b00              No wave generation (Normal mode)
			 */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);

			/*
			 * Timer/Counter Control Register1 B (TCCR1B)
			 * CS12:10=config_Ptr->clock   To select the desired prescaler
			 *
			 */
			TCCR1B = (TCCR1B & 0xF8) | (config_Ptr->clock);

			/*
			 * Timer/Counter1
			 * TCNT1=config_Ptr->initialValue   To initialize the timer by the desired value
			 */
			TCNT1 = config_Ptr->initialValue;

			/*
			 * Timer Interrupt Mask TIMSK
			 * TOIE1=1   To enable the overflow interrupt
			 */
			SET_BIT(TIMSK, TOIE1);

		}   /* End else if(config_Ptr->timer==TIMER1&&config_Ptr->mode==NORMAL) */
		else if(config_Ptr->mode == COMPARE)
		{
			/*
			 * Timer/Counter1 Control Register1 A (TCCR1A)
			 * COM1A1:0/COM1B1:0=0b0000   To select timer1 compare mode operation
			 * FOC1A/FOC1B=0b11           Non-PWM mode (compare mode)
			 * WGM11:10=0b00              No wave generation (compare mode)
			 */
			TCCR1A = (1<<FOC1A);

			/*
			 * Timer/Counter Control Register1 B (TCCR1B)
			 * CS12:10=config_Ptr->clock   To select the desired prescaler
			 * WGM10:13=0100,WGM12=1       To selclet Clear TCNT1 on Compare match mode(CTC mode)
			 */
			TCCR1B = (TCCR1B & 0xF8) | (config_Ptr->clock);
			SET_BIT(TCCR1B, WGM12);

			/*
			 * Timer/Counter1
			 * TCNT1=config_Ptr->initialValue   To initialize the timer by the desired value
			 */
			TCNT1L = config_Ptr->initialValue;
			TCNT1H = config_Ptr->initialValue;

			/*
			 * Output Compare Register 1 A OCR1A
			 * OCR1A=config_Ptr->compareValue   To set the compare match value in CTC
			 */
			OCR1A = config_Ptr->compareValue;

			/*
			 * Timer Interrupt Mask TIMSK
			 * OCIE1A=1   To enable the Output Compare A match interrupt
			 */
			SET_BIT(TIMSK, OCIE1A);
		}   /* End else if(config_Ptr->mode==COMPARE) */
	}
	else if(config_Ptr->timer == TIMER2 && config_Ptr->mode == NORMAL)
	{
	}   /* End if(config_ptr->timer==TIMER2&&config_Ptr->mode==NORMAL) */


}   /* End Timer_Init(const TimerConfig*config_Ptr) Function */

/* A function to stop all timers*/
void Timer_stop(TimerSelect timer)
{

	if(timer == TIMER1)
	{
		/* clear all timer1 registers */
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
	}   /* End if(timer==TIMER1) */
}   /* End Timer_stop() function */

/* A function to clear the selected timer value */
void Timer_clearValue(TimerSelect timer)
{

	if(timer == TIMER0)TCNT0 = 0;
	else if(timer == TIMER1)TCNT1 = 0;
	else if(timer == TIMER2)TCNT2 = 0;

}   /* End Timer_clearValue() function */

void Timer1_setCallBack(void(*a_Ptr)())
{
	g_callBackPtr = a_Ptr;
}   /* End Timer1_setCallBack() function */

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)(*g_callBackPtr)();
}   /* End ISR(TIMER1_COMPA_vect)Function */

ISR(TIMER0_OVF_vect)
{
	//if(g_callBackPtr != NULL_PTR)(*g_callBackPtr)();
}
