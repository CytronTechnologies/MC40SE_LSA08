/*******************************************************************************
* This file provides the functions for the interrupt service routine
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/



#include <htc.h>
#include "system.h"
#include "timer1.h"



/*******************************************************************************
* Interrupt Service Routine
*
* DESCRIPTIONS:
* This is the main ISR 
*
*******************************************************************************/
void interrupt isr(void)
{
	// check if Timer 1 is overflow
	if (TMR1IF == 1) 
	{		
		timer1_isr();		// call timer 1 ISR		
	}
	// User may develop their own ISR under here
}
