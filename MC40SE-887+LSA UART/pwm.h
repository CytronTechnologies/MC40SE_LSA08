/*******************************************************************************
* This file provides the functions for the PWM module.
*
* Author: Kong Wai Weng @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/



#ifndef _PWM_H
#define _PWM_H



/*******************************************************************************
* PUBLIC FUNCTION PROTOTYPES                                                   *
*******************************************************************************/

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
* Initialize the CCP1 module to operate in PWM mode.
*
*******************************************************************************/
extern void pwm_init(void);



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
extern void set_pwm1(unsigned int ui_duty_cycle);



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
extern void set_pwm2(unsigned int ui_duty_cycle);

#endif
