/*******************************************************************************
* This file provides the functions for the ADC module for MC40SE using PIC16F887
*
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/



#include <htc.h>
#include "system.h"
#include "adc.h"



/*******************************************************************************
* PUBLIC FUNCTION: adc_init
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the ADC module.
*
*******************************************************************************/
void adc_init(void)
{
#if defined(_16F887)//if this file is compile for PIC16F887
	// A/D Conversion Clock = FOSC/32.
	ADCS1 = 1;
	ADCS0 = 0;
	
	// Set AN0 as analog input only, the rest is digital I/O.
	ANS0 = 1;	// AN0 is analog input
	ANS1 = 0;	// AN1 is digital I/O
	ANS2 = 0;	// AN2 is digital I/O
	ANS3 = 0;	// AN3 is digital I/O
	ANS4 = 0;	// AN4 is digital I/O
	ANS5 = 0;	// AN5 is digital I/O
	ANS6 = 0;	// AN6 is digital I/O
	ANS7 = 0;	// AN7 is digital I/O
	ANS8 = 0;	// AN8 is digital I/O
	ANS9 = 0;	// AN9 is digital I/O
	ANS10 = 0;	// AN10 is digital I/O
	ANS11 = 0;	// AN11 is digital I/O
	ANS12 = 0;	// AN12 is digital I/O
	ANS13 = 0;	// AN13 is digital I/O

	// Configure the result to be right justified.
	ADFM = 1;
	
	// Configure Vref (-) to Vss
	VCFG1 = 0;

	// Configure Vref (+) to Vdd
	VCFG0 = 0;
	
	// Turn OFF ADC by default.
	ADON = 0;
#elif defined (_16F877A)	//if this file is compile for PIC16F877A
	// A/D Conversion Clock = FOSC/32.
	ADCS2 = 0;
	ADCS1 = 1;
	ADCS0 = 0;
	
	// Set AN0 as analog input only, the rest is digital I/O.
	PCFG3 = 1;	
	PCFG2 = 1;	
	PCFG1 = 1;	
	PCFG0 = 0;		

	// Configure the result to be right justified.
	ADFM = 1;	
	
	// Turn OFF ADC by default.
	ADON = 0;
#endif
}	



/*******************************************************************************
* PUBLIC FUNCTION: ui_adc_read
*
* PARAMETERS:
* void
*
* RETURN:
* ~ The ADC result in 16 bit
*
* DESCRIPTIONS:
* Convert and read the result of the ADC at channel 0.
*
*******************************************************************************/
extern unsigned int ui_adc_read(void)
{
	unsigned int temp = 0;
	// Select the ADC channel.
#if defined (_16F887)	// if this file is compile for PIC16F887
	CHS3 = 0;	//select channel 0
#endif
	CHS2 = 0;
	CHS1 = 0;
	CHS0 = 0;
	
	// Delay 5mS to fully charge the holding capacitor in the ADC module.
	__delay_ms(5);
	
	// Start the conversion and wait for it to complete.
	GODONE = 1;
	while (GODONE == 1);
	
	// Return the ADC result.
	temp = ADRESH << 8;
	temp = temp + ADRESL;
	return temp;
}	
