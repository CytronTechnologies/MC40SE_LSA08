/*******************************************************************************
* This is the header file that describes the system properties for MC40SE 
* using PIC16F887 or PIC16F877A
* Version: 1.1
* Date: 19 Sep 2011
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/
// For details connection, please refer to schematic of MC40SE
//================================== MC40SE PIN ASSIGNMENT ===========================
/*;PIN Des 	func			PIN Des 	func
;1	MCLR	Reset button	40	RB7		LED/BUZZER
;2	RA0		ADC				39	RB6		GPIO
;3	RA1		SEN7			38	RB5		SEN6
;4	RA2		SW1/GPIO 		37	RB4		SEN5
;5	RA3		SW2/GPIO		36	RB3		SEN4
;6	RA4		GPIO			35	RB2		SEN3
;7	RA5		SEN8			34	RB1		SEN2
;8	RE0		RUN1			33	RB0		SEN1
;9	RE1		DIR1			32	VDD		5V
;10	RE2		LCD_E			31	VSS		GND
;11	VDD		5V				30	RD7		LCD_DB7
;12	VSS		GND				29	RD6		LCD_DB6
;13	OSC1	BL_R/Crystal 	28	RD5		LCD_DB5
;14	OSC2	SK_R/Crystal 	27	RD4		LCD_DB4
;15	RC0		ENCODER			26	RC7		RX
;16	RC1		PWM1			25	RC6		TX
;17	RC2		PWM2			24	RC5		DIR2
;18	RC3		LATCH			23	RC4		RUN2
;19	RD0		Relay 1/LCD_RS	22	RD3		Relay 4
;20	RD1		Relay 2			21	RD2		Relay 3
*/
#ifndef _SYSTEM_H	// if this file has not been include, include again
#define _SYSTEM_H

// Oscillator Frequency.
#if defined (_16F887) // if this file is compile for PIC16F887
// user may choose 8MHz (internal crystal) or 20MHz (external crystal)
// if external crystal is use, ensure JP27 (BL_R) and JP48 (SK_R)
// MUST not be connected, else crystal will not work
//#define	_XTAL_FREQ		8000000		//using internal osc
#define	_XTAL_FREQ		8000000	//using external crystal

#elif defined (_16F877A)//if this file is compile for PIC16F877A
// PIC16F877A must use external crystal, ensure JP27 (BL_R) and JP48 (SK_R)
// MUST not be connected, else crystal will not work
#define	_XTAL_FREQ		20000000	//using external crystal
#endif


// I/O Connections.
// Parallel 2x16 Character LCD
#define LCD_E			RE2		// E clock pin is connected to RB5	
#define LCD_RS			RD0		// RS pin is used for LCD to differentiate data is command or character

#define	LCD_DATA		PORTD	// Data port of LCD is connected to PORTD, 4 bit mode								
								// Interface in 4-bit mode
								// LCD DB7 = RD7
								// LCD DB6 = RD6
								// LCD DB5 = RD5
								// LCD DB4 = RD4


// Push button on MC40SE
#define SW1				RA2		
#define SW2				RA3

// LED on MC40SE
#define LED1			RB7		// is being shared with buzzer

// Buzzer
#define BUZZER			RB7		// is being shared with LED


// Control pins for DC Brushless Motor
#define RUN1			RE0		//Brushless motor 1 Run pin
#define DIR1			RE1		//Brushless motor 1 direction pin
#define PWM1			CCPR1L	//Brushless motor 1 external speed
#define RUN2			RC4		//Brushless motor 2 Run pin
#define DIR2			RC5 	//Brushless motor 2 direction pin
#define PWM2			CCPR2L	//Brushless motor 2 external speed

#if defined (_16F887)
// PIC16F887 have RA7, PIC16F877A does not have, so this pin on define if PIC16F887 is use
#define BL_R			RA7		//Brushless motor reset pin, active low
								//can only use if external crystal is not use
#endif

// Digital Input pin assignment
#define SEN1			RB0		
#define SEN2			RB1
#define SEN3			RB2
#define SEN4			RB3
#define SEN5			RB4
#define SEN6			RB5
#define SEN7			RA1
#define SEN8			RA5

// Relay, to control brushed motor with full speed
#define LATCH			RC3	//pin that control 8 bit latch further control relay
#define RELAY1			RD0
#define RELAY2			RD1
#define RELAY3			RD2
#define RELAY4			RD3

// SK's Reset
#if defined (_16F887)
// PIC16F887 have RA7, PIC16F877A does not have RA6 and RA7, so if PIC16F887 is use define
#define SK_R			RA6		// pin to reset SK
								// can only be use if internal crystal is used
#endif
#endif