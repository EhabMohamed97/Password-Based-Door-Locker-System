/*
 * Moudle:NeXuSKeypad.c
 *  Created on:Nov 11, 2020
 *   Author:Ehab
 *    Hardware Abstraction Layer (HAL)
 */

#include"Keybad.h"

/* Private functions prototypes: */

#if(N_COL == 3)
   static uint8 Keypad_4x3_adjustSwitchNumber(uint8);
#elif(N_COL == 4)
   static uint8 Keypad_4x4_adjustSwitchNumber(uint8);
#endif


/* Public functions definitions: */
uint8 Keypad_getPressedKey()
{
	uint8 row,col;

	while(1)
	{

		for(col = 0; col < N_COL; col++ )
			{
				KEYPAD_PORT_DIR = (0b00010000<<col);   /* Every col is output and the rest are inputs */

				/* one col is cleared every iteration and the rest  have internal pull up resistors */
			    KEYPAD_PORT = (~(0b00010000<<col));

				for(row = 0; row < N_ROW; row++)
				{

					if(BIT_IS_CLEAR(KEYPAD_PIN,row))
					{

						#if(N_COL == 3)
		                   return Keypad_4x3_adjustSwitchNumber((row*N_COL)+col+1);
		                #elif(N_COL == 4)
		                   return Keypad_4x4_adjustSwitchNumber((row*N_COL)+col+1);
		                #endif
					}   /* End if */

				}   /* End for(row=0;row<N_ROW; row++)*/

			}   /* End for(col=0;col<N_COL;col++) */

	}   /* End while() */

 }   /* End Keypad_getPressedKey() function */

/* Private functions definitions: */

#if(N_COL ==3)
/*
 * A function to make the keypad fit it's 4x3
 * design in proteus
 */
static uint8 Keypad_4x3_adjustSwitchNumber(uint8 buttonNumber)
{

	switch(buttonNumber)
	{
	   case 10:
		   return'*';
		   break;
	   case 11:
		   return 0;
		   break;
	    case 12:
		    return'#';
		    break;
	    default:
		    return buttonNumber;
	}   /*End switch */

}   /* End Keypad_4x3_adjustSwitchNumber() function */
#elif(N_COL == 4)
/*
 * A function to make the keypad fit it's 4x4
 * design in proteus
 */
static uint8 Keypad_4x4_adjustSwitchNumber(uint8 buttonNumber)
{

	switch(buttonNumber)
	{
	   case 1:
		   return 7;
		   break;
	   case 2:
		   return 8;
		   break;
	    case 3:
		    return 9;
		    break;
	    case 4:
	    	return'/';
	    	break;
	    case 5:
	    	return 4;
	    	break;
	    case 6:
	    	return 5;
	    	break;
	    case 7:
	    	return 6;
	    	break;
	    case 8:
	    	return'*';
	    	break;
	    case 9:
	    	return 1;
	    	break;
	    case 10:
	    	return 2;
	    	break;
	    case 11:
	    	return 3;
	    	break;
	    case 12:
	    	return'-';
	    	break;
	    case 13:
	    	return 13;
	    	break;
	    case 14:
	    	return 0;
	    	break;
	    case 15:
	    	return'=';
	    	break;
	    case 16:
	    	return '+';
	    	break;
	}   /* End switch */

}   /* End Keypad_4x4_adjustSwitchNumber() function */
#endif
