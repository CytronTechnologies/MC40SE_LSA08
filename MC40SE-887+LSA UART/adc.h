/*******************************************************************************
* This file provides the functions for the ADC module for MC40SE using PIC16F887
*
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/



#ifndef _ADC_H
#define _ADC_H



/*******************************************************************************
* PUBLIC FUNCTION PROTOTYPES                                                   *
*******************************************************************************/

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
extern void adc_init(void);



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
extern unsigned int ui_adc_read(void);



#endif
