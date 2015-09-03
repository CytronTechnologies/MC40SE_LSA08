/*******************************************************************************
* This file provides the functions for the Timer1 module, use as counter for encoder input
* on MC40SE, PIC16F887
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/



#include <htc.h>
#include "system.h"
#include "timer1.h"



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES                                                     *
*******************************************************************************/

// The variables for the encoder.
static unsigned int encoder = 0;



/*******************************************************************************
* PUBLIC FUNCTION: timer1_init
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the Timer 1 module as counter, also enable it for interrupt
*
*******************************************************************************/
void timer1_init(void)
{
	TMR1L = 0x00;
	TMR1H = 0x00;	// clear value in timer 1, 16-bit register
	
	T1OSCEN = 0;	// Disable Timer 1 Oscillator.
	TMR1CS = 1;		// Select external clock.
	
	T1CKPS0 = 0;
	T1CKPS1 = 0;	// 1:: prescaler
	
	T1SYNC = 0;		// no sync
	TMR1IF = 0;		// Clear Timer 1 interrupt flag.
	TMR1IE = 1;		// Enable Timer 1 overflow interrupt.
	PEIE = 1;		// Enable all unmasked peripheral interrupts.
	GIE = 1;		// Enable all unmasked interrupts.
	
	TMR1ON = 1;		// Turn on Timer 1.
}	





/*******************************************************************************
* PUBLIC FUNCTION: ui_encoder
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ The value for encoder in 16-bit
*
* DESCRIPTIONS:
* Get the value for encoder in 16-bit.
*
*******************************************************************************/
unsigned int ui_encoder(void)
{
	encoder = ((unsigned int)(TMR1H)<<8) | TMR1L;

	return encoder;
}


/*******************************************************************************
* PUBLIC FUNCTION: set_encoder
*
* PARAMETERS:
* ~ ui_value	- The value for 16-bit encoder.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Set the value for 16-bit encoder.
*
*******************************************************************************/
void set_encoder(unsigned int ui_value)
{
	TMR1H = (unsigned char)(ui_value>>8);
	TMR1L = (unsigned char)ui_value;
}	


/*******************************************************************************
* Interrupt Service Routine for Timer 1
*
* DESCRIPTIONS:
* This is the ISR for the Timer 1 overflow interrupt, this is to serve encoder overflow.
* The ISR will only activate LED on MC40SE as indicator. User are free to modify the ISR
*
*******************************************************************************/
void timer1_isr(void)
{	
		// Clear the interrupt flag.
		TMR1IF = 0;		
		LED1 = 1;
}
