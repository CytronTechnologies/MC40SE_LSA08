/*******************************************************************************
* This file provides the functions for the PWM module on MC40SE.
*
* Author: Ober Choo @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/



#include <htc.h>
#include "system.h"
#include "pwm.h"



/*******************************************************************************
* PUBLIC FUNCTION: pwm_init
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the CCP1 and CCP2 module to operate in PWM mode.
*
*******************************************************************************/
void pwm_init(void)
{
	// Setting PWM frequency = 4.90KHz at 8MHz OSC Freq
	PR2 = 0xFF;
	T2CKPS1 = 0;
	T2CKPS0 = 1;	// Timer 2 prescale = 4.
	
	CCPR1L = 0;		// Duty cycle = 0;
	CCPR2L = 0;		// Duty cycle = 0;
	TMR2ON = 1;		// Turn on Timer 2.
	
	//configuration for CCP1CON
#if defined (_16F887)	//if this file is compile for PIC16F887
	P1M1 = 0;		//CCP1, P1A as single output
	P1M0 = 0;

	DC1B1 = 0;		// 2 LSB of 10-bit PWM, make it 00
	DC1B0 = 0;

	CCP1M3 = 1;		// Configure CCP1 module to operate in PWM mode.
	CCP1M2 = 1;		
	CCP1M1 = 0;
	CCP1M0 = 0;	
	
	//configuration for CCP2CON
	CCP2X = 0;		// 2 LSB of 10-bit PWM, make it 00
	CCP2Y = 0;

	CCP2M3 = 1;		// Configure CCP1 module to operate in PWM mode.
	CCP2M2 = 1;		
	CCP2M1 = 0;
	CCP2M0 = 0;	
#elif defined(_16F877A) //if this file is compile for PIC16F877A
	CCP1X = 0;		// 2 LSB of 10-bit PWM, make it 00
	CCP1Y = 0;

	CCP1M3 = 1;		// Configure CCP1 module to operate in PWM mode.
	CCP1M2 = 1;		
	CCP1M1 = 0;
	CCP1M0 = 0;	
	
	//configuration for CCP2CON
	CCP2X = 0;		// 2 LSB of 10-bit PWM, make it 00
	CCP2Y = 0;

	CCP2M3 = 1;		// Configure CCP1 module to operate in PWM mode.
	CCP2M2 = 1;		
	CCP2M1 = 0;
	CCP2M0 = 0;	
#endif
}	



/*******************************************************************************
* PUBLIC FUNCTION: set_pwm1
*
* PARAMETERS:
* ~ ui_duty_cycle	- The duty cycle of the PWM1 in 16-bit, 10-bit significant.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Set the duty cycle of the PWM1.
*
*******************************************************************************/
void set_pwm1(unsigned int ui_duty_cycle)
{
	CCP1CON = (CCP1CON & 0b11001111) | (0b00110000 & ((unsigned char)(ui_duty_cycle << 4)));
	CCPR1L = ui_duty_cycle >> 2;
}	

/*******************************************************************************
* PUBLIC FUNCTION: set_pwm2
*
* PARAMETERS:
* ~ ui_duty_cycle	- The duty cycle of the PWM2 in 16-bit, 10-bit significant.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Set the duty cycle of the PWM2.
*
*******************************************************************************/
void set_pwm2(unsigned int ui_duty_cycle)
{
	CCP2CON = (CCP2CON & 0b11001111) | (0b00110000 & ((unsigned char)(ui_duty_cycle << 4)));
	CCPR2L = ui_duty_cycle >> 2;
}	
