/*******************************************************************************
* This file provides the functions for the SKPS for MC40SE using PIC16F887
*
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/



#include <htc.h>
#include "system.h"
#include "skps.h"
#include "uart.h"



/*******************************************************************************
* PUBLIC FUNCTION: uc_skps_status
*
* PARAMETERS:
* ~ uc_data
*
* RETURN:
* ~ data received from SKPS, the status 
*
* DESCRIPTIONS:
* request SKPS button and joystick status
*
*******************************************************************************/
unsigned char uc_skps(unsigned char uc_data)
{
	// send command to request PS2 status
	uart_tx(uc_data);
	return uc_uart_rx();
}	



/*******************************************************************************
* PUBLIC FUNCTION: skps_vibrate
*
* PARAMETERS:
* uc_motor, uc_value
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* command SKPS to vibrate motor on PS2 controller.
*
*******************************************************************************/
void skps_vibrate(unsigned char uc_motor, unsigned char uc_value)
{
	uart_tx(uc_motor);		//send number of motor, motor 1 or motor 2 to SKPS
	uart_tx(uc_value);		//send the speed, activate or deactivate command to SKPS	
}	


/*******************************************************************************
* PUBLIC FUNCTION: skps_reset
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* reset the SKPS and later release it
*
*******************************************************************************/
void skps_reset(void)
{
#if defined(_16F887)	// PIC16F887 have RA6, PIC16F877A does not have
	SK_R = 1; 			// reset the SKPS
	__delay_ms(20);		// wait for 20 ms
	SK_R = 0;			// release reset, SKPS back to normal operation
	__delay_ms(20);	
#endif
	__delay_ms(20);
}	
