/*******************************************************************************
* This is the sample main program for MC40SE Advance Mobile Robot Controller 
* using PIC16F887, compiled with MPLAB IDE v8.63 + HITECH C Compiler V9.80 Lite mode
* Line Following sample
* Revision: 1.0
* Date: 21 Feb 2012
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
* We do not hold resposibility for the working of this sample code, there are many posibilities for the code
* not to work, motor type, gear ratio, speed, tires size, sensor position, calibration, etc
* If you have problem, please come to our forum to discuss
* http://forum.cytron.com.my
*******************************************************************************/
#include <htc.h>		// include the HiTECH C Header
#include "system.h"		// header of hardware pin, Crystal speed, baud rate
#include "timer1.h"		// header file for timer1, use as counter for encoder
#include "uart.h"		// header file for UART, serial communication
#include "adc.h"		// header file for ADC
#include "pwm.h"		// header file for PWM, speed control
#include "lcd.h"		// header file for LCD
#include "skps.h"		// header file for SKPS

/*******************************************************************************
* DEVICE CONFIGURATION WORDS FOR PIC16F887                                     *
*******************************************************************************/
#if defined (_16F887)	// if the PIC selected is PIC16F887
__CONFIG( INTIO &		// Internal Clock, pins as I/O.
		 //HS &			// External Crystal at High Speed
		 WDTDIS &		// Disable Watchdog Timer.
		 PWRTEN &		// Enable Power Up Timer.
		 IESOEN &			// Enabled Internal External Clock Switch Over
		 //IESODIS &		// Disabled Internal External Clock Switch Over
		 BORDIS &		// Disable Brown Out Reset.
		 //FCMDIS	&		// Disable monitor clock fail safe
		 FCMEN &		// Enable monitor clock fail safe
		 MCLREN &		// MCLR function is enabled
		 LVPDIS);		// Disable Low Voltage Programming.

#elif defined (_16F877A)// if PIC16F877A is selected, 
//ensure JP27 (BL_R) and JP48 (SK_R) is not connected
__CONFIG(HS &			// External Crystal at High Speed
		 WDTDIS &		// Disable Watchdog Timer
		 PWRTEN &		// Enable Power Up Timer		 
		 BORDIS &		// Disable Brown Out Reset		 
		 LVPDIS);		// Disable Low Voltage Programming
#endif	
/*******************************************************************************
* PRE LOAD DATA TO EEPROM
********************************************************************************/
__EEPROM_DATA(1,0,0,0,0,0,0,0);		//Load 1 to 0x00, and 7 bytes of data in EEPROM, it must be 8 bytes

/*
extern void eeprom_write(unsigned char addr, unsigned char value);
extern unsigned char eeprom_read(unsigned char addr);
*/

/*******************************************************************************
* PRIVATE CONSTANT DEFINE                                                  *
*******************************************************************************/
#define	PORT1 	1
#define	PORT2	2
#define	RUN		0
#define	BRAKE	1
#define	CW		0
#define	CCW		1		
#define SPEED	500		// initial constant speed as 500
#define PIVOT_SPEED	 450	// speed during pivot, left and right motor will use same speed

/* Gear type, this is due to different gear type, the rotation will be different
* this will further affect the locomotion, please choose the correct gear type
*
* type 0 is applied for:
*Linix Brushless Motor:(XXW - YY ---> XX watt for motor power, YY for gear ratio)
*	a. 30W - 10 
*Vexta Brushless Motor:(XXW - YY ---> XX watt for motor power, YY for GEAR ratio)
*	a. 15W - 20, 30, 200
*	b. 30W - 30, 50, 100
*	c. 50W - 30, 50, 100
*
*type 1 is applied for:
*Linix Brushless Motor:(XXW - YY ---> XX watt for motor power, YY for gear ratio)
*	a. 10W - 5, 15
*	b. 30W - 20
*Vexta Brushless Motor:(XXW - YY ---> XX watt for motor power, YY for gear ratio)
*	a. 15W - 5, 10, 15, 50, 100
*	b. 30W - 5, 10, 15, 20, 200
*	c. 50W - 5, 10, 15, 20, 200
*/
#define GEAR			1				// gear type

#define left_speed(a)	set_pwm1(a)		// left motor at PORT1
#define right_speed(a) 	set_pwm2(a)		// right motor at PORT2
#define DIRL			DIR1			// direction pin for Left motor
#define	DIRR			DIR2			// direction pin for Right motor
#define RUNL			RUN1			// RUN/BRAKE pin for Left motor
#define RUNR			RUN2			// RUN/BRAKE pin for Right motor

// sensor port for line following
#define	S1			SEN1			// left sensor at SEN1 port
#define S2			SEN2			// middle sensor at SEN2 port
#define	S3			SEN3			// rigth sensor at SEN3 port
#define S4			SEN4			// cross sensor at SEN4 port
#define	S5			SEN5			// left sensor at SEN1 port
#define S6			SEN6			// middle sensor at SEN2 port
#define	S7			SEN7			// rigth sensor at SEN3 port
#define S8			SEN8			// cross sensor at SEN4 port

/*******************************************************************************
* PRIVATE FUNCTION PROTOTYPES                                                  *
*******************************************************************************/
void delay_ms(unsigned int ui_value);
void beep(unsigned char uc_count);
void mc40se_init(void);

void brushless(unsigned char uc_port_number, unsigned char uc_motor_status, unsigned char uc_motor_dir, unsigned int ui_speed);
void brush(unsigned char uc_port_number, unsigned char uc_motor_status, unsigned char uc_motor_dir, unsigned int ui_speed);
void reset_brushless(void);
void relay_on(unsigned char uc_relay_number);
void relay_off(unsigned char uc_relay_number);
void relay_off_all(void);

//brushless motor
void forward(void);
void reverse(void);
void left(void);
void right(void);
void stop(void);
void run(void);
void motorspeed(unsigned int m1, unsigned int m2);
void LF();

// function for line following
void line_following(void);
void line_follow_cross(unsigned char cross_count);
void pivot_left(unsigned char cross_count);
void pivot_right(unsigned char cross_count);


/*******************************************************************************
* Global Variables                                                             *
*******************************************************************************/
unsigned int left_s = 0, right_s = 0;	//motor speed
unsigned char different = 0;
/*******************************************************************************
* MAIN FUNCTION                                                                *
*******************************************************************************/
int main(void)
{
	
	// Initialize PIC16F887 to correct Input/Output based on MC40SE on board interface
	mc40se_init();
	
	// Initialize PWM.
	pwm_init();
	
	// off all relays
	relay_off_all();	
	
	// Initialize ADC.
	adc_init();	
	
	// Initialize UART.
	uart_init();
	
	// Initialize PWM.
	timer1_init();
	
	// Initialize the LCD.
	lcd_init();		
	
	// Initialize bruhsless motor port, if you are using brush motor, please use other function. 
	brushless(PORT1, BRAKE, CW, 0);
	brushless(PORT2, BRAKE, CCW, 0);
			
	// Display the messages and beep twice.		
	lcd_clear_msg(" MC40SE\n LSA08");
	delay_ms(1000);	
		
	beep(2);
	lcd_clear_msg("  Line\nFollow");
	
	line_following();	//go to line_following function to see the details
	
	lcd_clear_msg("Done!");	
	beep(5);	
	
	while(1) continue;	// inifinate while loop to prevent reset at the end of program
}



/*******************************************************************************
* PRIVATE FUNCTION: delay_ms
*
* PARAMETERS:
* ~ ui_value	- The period for the delay in miliseconds.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Delay in miliseconds.
*
*******************************************************************************/
void delay_ms(unsigned int ui_value)
{
	while (ui_value-- > 0) {
		__delay_ms(1);
	}	
}


/*******************************************************************************
* PRIVATE FUNCTION: beep
*
* PARAMETERS:
* ~ uc_count	- How many times we want to beep.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Beep for the specified number of times.
*
*******************************************************************************/
void beep(unsigned char uc_count)
{
	while (uc_count-- > 0) {
		BUZZER = 1;
		delay_ms(50);
		BUZZER = 0;
		delay_ms(50);
	}
}
/*******************************************************************************
* PRIVATE FUNCTION: mc40se_init
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the PIC16F887 for MC40SE on board device.
*
*******************************************************************************/
void mc40se_init(void)
{
#if defined (_16F887)
	// Initialize the Internal Osc, under OSCCON register
	IRCF2 = 1;		// IRCF<2:0> = 111 => 8MHz
	IRCF1 = 1;		// IRCF<2:0> = 110 => 4MHz, default
	IRCF0 = 1;		// IRCF<2:0> = 101 => 2MHz, 
#endif		
	// clear port value
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;
	
	// Initialize the I/O port direction.
	TRISA = 0b00111111;
	TRISB = 0b00111111;
	TRISC = 0b10000001;
	TRISD = 0;
	TRISE = 0b00000000;	
}

/*******************************************************************************
* PRIVATE FUNCTION: brushless
*
* PARAMETERS:
* ~ unsigned char uc_port_number - refer to brushless port on MC40SE, PORT1 or PORT2
* ~ unsigned char uc_motor_status - refer to either RUN or BRAKE brushless motor
* ~ unsigned char uc_motor_dir - to control the direction of brushless motor, CW or CCW
* ~ unsigned int ui_speed - speed for brushless motor, from 0 to 1024
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* control brushless motor, suitable for Vexta or LINIX brushless motor connect to MC40SE
*
*******************************************************************************/
void brushless(unsigned char uc_port_number, unsigned char uc_motor_status, unsigned char uc_motor_dir, unsigned int ui_speed)
{
	if(uc_port_number == PORT1)
	{
		if(uc_motor_status == RUN)
		{		
			RUN1 = 0;	// active low		
		}
		else if(uc_motor_status == BRAKE)
		{
			RUN1 = 1;	// activate low	
			BL_R = 0;	//after stop, reset the brushless
			__delay_ms(5);	// this reset can only be used if internal crystal is used
			BL_R = 1;		// and JP27 (BL-R) is connected
			__delay_ms(10);				
		}
		
		if(uc_motor_dir == CW)
		{
			DIR1 = 1;
		}
		else if (uc_motor_dir == CCW)
		{
			DIR1 = 0;
		}
		set_pwm1(ui_speed);
	}
	else if(uc_port_number == PORT2)
	{
		if(uc_motor_status == RUN)
		{		
			RUN2 = 0;	// active low		
		}
		else if(uc_motor_status == BRAKE)
		{
			RUN2 = 1;	// activate low	
			BL_R = 0;	//after stop, reset the brushless
			__delay_ms(5);	// this reset can only be used if internal crystal is used
			BL_R = 1;		// and JP27 (BL-R) is connected
			__delay_ms(10);				
		}
		
		if(uc_motor_dir == CW)
		{
			DIR2 = 1;
		}
		else if (uc_motor_dir == CCW)
		{
			DIR2 = 0;
		}
		set_pwm2(ui_speed);
	}
}	
/*******************************************************************************
* PRIVATE FUNCTION: reset_brushless
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* reset alarm on Brushless motor, both port. Uses RA7 which is one of OSC pin.
* This function can only work if PIC is using internal oscillator.
* Jumper 27 (BL_R) must be connected.
*******************************************************************************/
void reset_brushless(void)
{
	#if defined (_16F887)
	BL_R = 0;
	__delay_ms(5);
	BL_R = 1;
	__delay_ms(10);	
	#endif	
}
/*******************************************************************************
* PRIVATE FUNCTION: brush
*
* PARAMETERS:
* ~ unsigned char uc_port_number - refer to brush port in MC40SE, PORT1 or PORT2
* ~ unsigned char uc_motor_status - refer to either RUN or BRAKE brush motor
* ~ unsigned char uc_motor_dir - to control the direction of brush motor, CW or CCW
* ~ unsigned int ui_speed - speed for brush motor, from 0 to 1024
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* control brush motor, require MD10X or MD30X as motor driver
*
*******************************************************************************/
void brush(unsigned char uc_port_number, unsigned char uc_motor_status, unsigned char uc_motor_dir, unsigned int ui_speed)
{
	if(uc_port_number == PORT1)
	{
		if(uc_motor_status == RUN)
		{
			if(uc_motor_dir == CW)
			{
				RUN1 = 0;
				DIR1 = 1;
			}
			else if (uc_motor_dir == CCW)
			{
				RUN1 = 1;
				DIR1 = 0;
			}			
		}
		else if(uc_motor_status == BRAKE)
		{
			RUN1 = 0;	// brake to gnd
			DIR1 = 0;	
		}
		set_pwm1(ui_speed);
	}
	else if(uc_port_number == PORT2)
	{
		if(uc_motor_status == RUN)
		{
			if(uc_motor_dir == CW)
			{
				RUN2 = 0;
				DIR2 = 1;
			}
			else if (uc_motor_dir == CCW)
			{
				RUN2 = 1;
				DIR2 = 0;
			}			
		}
		else if(uc_motor_status == BRAKE)
		{
			RUN2 = 0;	// brake to gnd
			DIR2 = 0;	
		}
		set_pwm2(ui_speed);
	}	
}	
/*******************************************************************************
* PRIVATE FUNCTION: relay_on
*
* PARAMETERS:
* ~ unsigned char uc_relay_number - relay that needed to be activate, from 1-8
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* activate relay.
*
*******************************************************************************/
void relay_on(unsigned char uc_relay_number)
{
	LATCH = 0;	// hold the state of latch
	if(uc_relay_number > 0)
	{
	PORTD = PORTD | (0b00000001<< (uc_relay_number - 1));
	LATCH = 1;	// transfer the new output to relay
	delay_ms(1);
	LATCH = 0;	// hold the output of latch
	}
}	
/*******************************************************************************
* PRIVATE FUNCTION: relay_off
*
* PARAMETERS:
* ~ unsigned char uc_relay_number - relay that needed to be deactivate, from 1-8
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Deactivate relay
*
*******************************************************************************/
void relay_off(unsigned char uc_relay_number)
{
	LATCH = 0;	// hold the state of latch
	if(uc_relay_number > 0)
	{
	PORTD = PORTD & (~(0b00000001<< (uc_relay_number -1)));
	LATCH = 1;	// transfer the new output to relay
	delay_ms(1);
	LATCH = 0;	// hold the output of latch
	}
}	
/*******************************************************************************
* PRIVATE FUNCTION: relay_off_all
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* deactivate all relays.
*
*******************************************************************************/
void relay_off_all(void)
{
	LATCH = 0;	// hold the output of latch
	PORTD = 0; 	// Off all relays
	LATCH = 1;	// enable latch to read input and transfer to output
	delay_ms(5);
	LATCH = 0;	// hold the output of latch
}
	

/*******************************************************************************
* PRIVATE FUNCTION: line_following
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Demo of line following using 3 fiber optic sensor at SEN1, SEN2 and SEN3, it will also 
* use SEN4 for cross counting. This function will line follow for 2 cross and turn 180 degree and continue until 10 times
* Please modify accordingly for your own purpose. Cytron Technologies do not hold the resposibilities to ensure the working
* of this sample code as line following require many factors, sensor calibration, sensor position, 
* motor direction and position, etc. You will need to modify accordingly.
* You are welcome to discuss in our technical forum at http://forum.cytron.com.my
*
*******************************************************************************/

void line_following(void)
{	
	unsigned char times = 0,cou = 0;	// loop iteration
	forward();	//forward the robot
	lcd_clear_msg("SW1 to\nStart!");
	delay_ms(100);	
	
	while(SW1 == 1) continue;	//wait for SW1 to be press
	delay_ms(10);	
	while(SW1 == 0) continue;	//wait for SW1 to be release
	
	lcd_clear_msg(" Line\nFollow");
	beep(1);
	times = 4; // will keep looping 4 times
	while(times > 0)
	{	
		line_follow_cross(3);
		beep(2);
		delay_ms(200);
		lcd_clear_msg("turn");
		brushless(PORT2, RUN, CW, 500);	
		brushless(PORT1, RUN, CW, 500);
		while(S8==0);
		while(S8==1);
		times --;
	}//	while(times > 0)
	brushless(PORT1, BRAKE, CW, 0);	//brake both motor
	brushless(PORT2, BRAKE, CCW, 0);
}

/*******************************************************************************
* PRIVATE FUNCTION: line_follow_cross
*
* PARAMETERS:
* ~ unsigned char cross_count
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* This function will call line following cross and will following line base on SEN1, 2 and 3
* SEN4 is use for cross counting.
* We are using fiber optic sensor, White line will activate, Digital pin will receive logic 0 or low
*******************************************************************************/
void line_follow_cross(unsigned char cross_count)
{
	unsigned char current_cross = 0,wait = 0;
	//need to move the robot to move forward a bit	
	brushless(PORT1, BRAKE, CW, left_s);	
	brushless(PORT2, BRAKE, CCW, right_s);
	while (cross_count > current_cross)
	{
		//forward movement
		if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 1)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{			
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg(" forward");	
		}
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 1)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
	   		brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg(" forward");	
		}	
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 1)&&(S5 == 1)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg(" forward");	
		}
		
		
		//move left in those condition
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 1)&&(S4 == 1)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 500);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left  ");	
		}
		else if((S1 == 0)&&(S2 == 1)&&(S3 == 1)&&(S4 == 1)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 500);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left1  ");	
		}	
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 1)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 400);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left2  ");	
		}	
		else if((S1 == 0)&&(S2 == 1)&&(S3 == 1)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 300);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left3  ");	
		}
		else if((S1 == 1)&&(S2 == 1)&&(S3 == 1)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 200);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left4  ");	
		}	
		else if((S1 == 0)&&(S2 == 1)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 100);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left5  ");
		}	
		else if((S1 == 1)&&(S2 == 1)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 0);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left6  ");	
		}	
		else if((S1 == 1)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 0);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left7  ");	
		}	
		
		
		//move right in those condition
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 1)&&(S6 == 1)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 500);
				lcd_clear_msg("right  ");	
		}
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 1)&&(S6 == 1)&&(S7 == 1)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 500);
				lcd_clear_msg("right1  ");	
		}
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 1)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 400);
				lcd_clear_msg("right2  ");	
		}	
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 1)&&(S7 == 1)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 300);
			lcd_clear_msg("right3  ");	
		}	
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 1)&&(S7 == 1)&&(S8 == 1))	
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 200);
			lcd_clear_msg("right4  ");	
		}
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 1)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 100);
			lcd_clear_msg("right5  ");	
		}
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 1)&&(S8 == 1))
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 0);
			lcd_clear_msg(" right6  ");	
		}	
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 1))	
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 0);
			lcd_clear_msg("right7  ");	
		}
		
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT1, BRAKE, CW, left_s);	
			brushless(PORT2, BRAKE, CCW, right_s);
			lcd_clear_msg("stop  ");	
		}
		
		//cross count system
		if((S1 == 1)&&(S2 == 1)&&(S3 == 1)&&(S4 == 1)&&(S5 == 1)&&(S6 == 1)&&(S7 == 1)&&(S8 == 1))	// if cross sensor detected cross
		{
			if((cross_count - 1) <= current_cross)	 break;	// if this is last cross, break from line following
															// loop and stop robot
			while(S8 == 1) continue;	//wait until the cross sensor is out of line
			current_cross ++;		
		}
	
	}//while (cross > cur_cross)
	//brushless(PORT1, BRAKE, CW, left_s);	//brake both motor
	//brushless(PORT2, BRAKE, CCW, right_s);
//	brushless(PORT1, BRAKE, CW, left_s);	
//	brushless(PORT2, BRAKE, CCW, right_s);
//	stop();
for(wait=0;wait<=10;wait++)
{
LF();
delay_ms(200);
}
}

/*******************************************************************************
* PRIVATE FUNCTION: pivot_left
*
* PARAMETERS:
* ~ unsigned char cross_count
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* This function pivot left and stop after detection of cross_count
*******************************************************************************/
void pivot_left(unsigned char cross_count)
{
	unsigned char current_cross = 0;	
	delay_ms(500);
	left();		// start turning left
	motorspeed(PIVOT_SPEED, PIVOT_SPEED);
	delay_ms(500);

	while(current_cross < cross_count)
	{
//		if(LEFT == 0)
		{
			if(current_cross >= (cross_count-1))	// if this is last cross
			motorspeed(PIVOT_SPEED - 100, PIVOT_SPEED - 100);	//slow down the pivot
			
//			while(LEFT == 0) continue;
			current_cross ++;
		}	
	}
	//	brushless(PORT1, BRAKE, CW, left_s);	//brake both motor
	//brushless(PORT2, BRAKE, CCW, right_s);
	stop();
	delay_ms(300);
}			

/*******************************************************************************
* PRIVATE FUNCTION: pivot_right
*
* PARAMETERS:
* ~ unsigned char cross_count
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* This function pivot right and stop after detection of cross_count
*******************************************************************************/
void pivot_right(unsigned char cross_count)
{
	unsigned char current_cross = 0;	
	delay_ms(500);
	right();		// start turning left
	motorspeed(PIVOT_SPEED, PIVOT_SPEED);
	delay_ms(500);

	while(current_cross < cross_count)
	{
//		if(RIGHT == 0)
		{
			if(current_cross >= (cross_count - 1))	// if the is last cross
			motorspeed(PIVOT_SPEED - 100, PIVOT_SPEED - 100);	//slow down the pivot
			
//			while(RIGHT == 0) continue;
			current_cross ++;
		}	
	}
	//brushless(PORT1, BRAKE, CW, left_s);	//brake both motor
	//brushless(PORT2, BRAKE, CCW, right_s);
	stop();
	delay_ms(300);
}			
// ==================== brushless motor control =======================================
//control brushless motor
//==============================================================================================
void forward(void)
{
	run();
	if(GEAR == 1)
	{
	DIRL = 1;	//motor at left(PORT1) and right(PORT2) 
	DIRR = 0;	//Robot will forward depend on gear type
	}
	else
	{
	DIRL = 0;	//motor at left(PORT1) and right(PORT2) 
	DIRR = 1;	//Robot will forward depend on gear type
	}
}

void reverse(void)
{
	run();
	if(GEAR == 1)
	{
	DIRL = 0;	//motor at left(PORT1) and right(PORT2) 
	DIRR = 1;	//Robot will reverse depend on gear type
	}
	else 
	{
	DIRL = 1;	//motor at left(PORT1) and right(PORT2) 
	DIRR = 0;	//Robot will reverse depend on gear type
	}	
}

void left(void)
{
	run();
	if(GEAR == 1)
	{
	DIRL = 0;	//motor at left(PORT1) and right(PORT2) 
	DIRR = 0;	//Robot will turn left depend on gear type		
	}
	else
	{
	DIRL = 1;	//motor at left(PORT1) and right(PORT2) 
	DIRR = 1;	//Robot will turn left depend on gear type		
	}	
}

void right(void)
{
	run();
	if(GEAR == 1)
	{
	DIRL = 1;	//motor at left(PORT1) and right(PORT2) 
	DIRR = 1;	//Robot will turn right depend on gear type	
	}
	else
	{	
	DIRL = 0;	//motor at left(PORT1) and right(PORT2) 
	DIRR = 0;	//Robot will turn right depend on gear type	
	}	
}
void stop(void)
{
	RUNL = 1;	//motor at left(PORT1) and right (PORT2)
	RUNR = 1; 	//will brake
	BL_R = 0;	//after stop, reset the brushless
	__delay_ms(5);	// this reset can only be used if internal crystal is used
	BL_R = 1;		// and JP27 (BL-R) is connected
	__delay_ms(10);	
}
void run(void)
{
	RUNL = 0;	//motor at left(PORT1) and right (PORT2)
	RUNR = 0; 	//will run	
}
void motorspeed(unsigned int m1, unsigned int m2)
{	
	left_speed(m1);
	right_speed(m2);	
}


void LF()
	{
		//forward movement
		if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 1)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{			
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg(" forward");	
		}
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 1)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
	   		brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg(" forward");	
		}	
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 1)&&(S5 == 1)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg(" forward");	
		}
		
		
		//move left in those condition
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 1)&&(S4 == 1)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 500);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left  ");	
		}
		else if((S1 == 0)&&(S2 == 1)&&(S3 == 1)&&(S4 == 1)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 500);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left1  ");	
		}	
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 1)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 400);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left2  ");	
		}	
		else if((S1 == 0)&&(S2 == 1)&&(S3 == 1)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 300);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left3  ");	
		}
		else if((S1 == 1)&&(S2 == 1)&&(S3 == 1)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 200);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left4  ");	
		}	
		else if((S1 == 0)&&(S2 == 1)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 100);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left5  ");
		}	
		else if((S1 == 1)&&(S2 == 1)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 0);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left6  ");	
		}	
		else if((S1 == 1)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 0);	
			brushless(PORT1, RUN, CCW, 600);
			lcd_clear_msg("left7  ");	
		}	
		
		
		//move right in those condition
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 1)&&(S6 == 1)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 500);
				lcd_clear_msg("right  ");	
		}
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 1)&&(S6 == 1)&&(S7 == 1)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 500);
				lcd_clear_msg("right1  ");	
		}
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 1)&&(S7 == 0)&&(S8 == 0))
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 400);
				lcd_clear_msg("right2  ");	
		}	
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 1)&&(S7 == 1)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 300);
			lcd_clear_msg("right3  ");	
		}	
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 1)&&(S7 == 1)&&(S8 == 1))	
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 200);
			lcd_clear_msg("right4  ");	
		}
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 1)&&(S8 == 0))	
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 100);
			lcd_clear_msg("right5  ");	
		}
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 1)&&(S8 == 1))
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 0);
			lcd_clear_msg(" right6  ");	
		}	
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 1))	
		{
			brushless(PORT2, RUN, CW, 600);	
			brushless(PORT1, RUN, CCW, 0);
			lcd_clear_msg("right7  ");	
		}
		
		else if((S1 == 0)&&(S2 == 0)&&(S3 == 0)&&(S4 == 0)&&(S5 == 0)&&(S6 == 0)&&(S7 == 0)&&(S8 == 0))	
		{
			brushless(PORT1, BRAKE, CW, left_s);	
			brushless(PORT2, BRAKE, CCW, right_s);
			lcd_clear_msg("stop  ");	
		}
	}	