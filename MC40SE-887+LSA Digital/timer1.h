/*******************************************************************************
* This file provides the functions for the Timer1 module, use as counter for encoder input
* on MC40SE, PIC16F887
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/



#ifndef _TIMER1_H
#define _TIMER1_H



/*******************************************************************************
* PUBLIC FUNCTION PROTOTYPES                                                   *
*******************************************************************************/

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
extern void timer1_init(void);



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
extern unsigned int ui_encoder(void);


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
extern void set_encoder(unsigned int ui_value);


/*******************************************************************************
* Interrupt Service Routine for Timer 1
*
* DESCRIPTIONS:
* This is the ISR for the Timer 1 overflow interrupt, this is to serve as Real Time Clock.
*
*******************************************************************************/
extern void timer1_isr(void);

#endif
