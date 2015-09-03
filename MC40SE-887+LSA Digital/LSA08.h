/*THIS SOFTWARE IS PROVIDED BY THE PROVIDER."AS IS" AND "WITH ALL FAULTS." THE
PROVIDER MAKES NO REPRESENTATIONS OR WARRANTIES OF ANY KIND CONCERNING THE 
SAFETY, SUITABILITY, INACCURACIES, TYPOGRAPHICAL ERRORS, OR OTHER HARMFUL
COMPONENTS OF THIS SOFTWARE. CYTRON TECHNOLOGIES SHALL NOT, IN ANY
CIRCUMSTANCES,BE LIABLE FOR SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES,
FOR ANY REASON WHATSOEVER.
*/
/*
LSA08 Library Function
LSA08.h
author: IH
cytron R&D

 */

//Function Prototypes
//user call functions

/*  char LSA08_Init(void)---------------------------------------------------------
 *  Function: to initialize LSA08
 *      if defined LSA08_USE_UART
 *         1. init the main controller UART module
 *         2. set uart mode to defined LSA08_UARTMODE
 *         3. clear the junction counter of LSA08
 *      if defined LSA08_USE_ANALOG
 *         1. init the main controller ADC module
 *
 *  Return: initiliazation status (1-sucessful 0- fail)
 */
char LSA08_Init(void);

/*  signed char LSA08_GetPosition(void)-----------------------------------------
 *  Function: to get the line position detected by LSA08
 *  Return:
 *      if defined LSA08_USE_UART and LSA08_UARTMODE=2
 *         1. return position of the line detected, value -35 to 35, middle as 0
 *         2. return 127 if no line detected
 *      if defined LSA08_USE_ANALOG
 *         1. return values dependent on defined ANALOG_MIDVAL (-values to +values)
 *         2. analog values from LSA08 range from 0 to 4.5v if line detected and
 *             5V if no line detect.
 *         3. return 127 if no line detected
 */
signed char LSA08_GetPosition(void);

/*unsigned char LSA08_GetSensor(void)-------------------------------------------
 * Function: to get sensors digital values, for LSA08_UARTMODE=1 only
 * Return: return all 8 sensors digital values. the sensor values are digitized
 * according to the THRESHOLD values of the setting of LSA08
 * MSB represent sensor 7 and LSB represent sensor 0.
 */
unsigned char LSA08_GetSensor(void);

/*unsigned char LSA08_GetJunction(void)-----------------------------------------
 * Function: to get junction count of LSA08
 * Return: Number of junctions detected so far
 */
unsigned char LSA08_GetJunction(void);

/*char LSA08_ClearJunction(void)------------------------------------------------
 * Function: to clear the junction count buffer of LSA08
 * Return: status (1-sucessful, 0-fail)
 */
char LSA08_ClearJunction(void);

/*char LSA08_Calibrate(void)----------------------------------------------------
 * Function: Call LSA08 to calibrate the sensor to the line and background
 * Return: Status (1-sucessful, 0-fail)
 */
char LSA08_Calibrate(void);

/*char LSA08_UARTMode(unsigned char mode)---------------------------------------
 * Function: to set the UART mode of LSA08;
 * Input: UART mode (0-3)
 *         1. mode 0: no data from UART of LSA08
 *         2. mode 1: one byte digital values of LSA08 sensors
 *         3. mode 2: Line position detected by LSA08
 *         4. mode 3: raw analog data of every sensor
 * Return: Status (1-sucessful, 0-fail)
 */
char LSA08_UARTMode(unsigned char mode);

/*char LSA08_JunctionWidth(unsigned char jw)------------------------------------
 * Function: to set the junction width i.e no of sensors detecting line
 *           which LSA08 consider as a junction cross. Set value to a value 
 *           which is 1 or 2 sensors more than the normal line width.
 * Input: Junction With/No of sensors (1-8)
 * Return: Status (1-sucessful, 0-fail)
 */
char LSA08_JunctionWidth(unsigned char jw);

/*char LSA08_Threshold(unsigned char threshold)---------------------------------
 * Function: to set the threshold value for the digitization of sensor value.
 *           This values determined how many bars as indicated in bar diagram of
 *           LCD, LSA08 will consider a line detected.
 *           set appropriate high threshold level for a stable operation of LSA08
 * Input: THRESHOLD value (0-7)
 * Return: Status (1-sucessful, 0-fail)
 */
char LSA08_Threshold(unsigned char threshold);

/*char LSA08_LineMode(unsigned char mode)---------------------------------------
 * Function: to set line mode of LSA08 either dark on or light on mode
 * Input: DARK_ON or LIGHT_ON
 * Return: Status (1-sucessful, 0-fail)
 */
char LSA08_LineMode(unsigned char mode);

/*char LSA08_Contrast(unsigned char contrast)-----------------------------------
 * Function: to set LCD display contrast value
 * Input: contrast level (0-255)
 * Return: Status (1-sucessful, 0-fail)
 */
char LSA08_Contrast(unsigned char contrast);

/*char LSA08_BL(unsigned char BL)-----------------------------------------------
 * Function: to set the Backlight level of LCD
 * Input: Backlight level (0-10)
 * Return: Status (1-sucessful, 0-fail)
 */
char LSA08_BL(unsigned char BL);


/*LSA08 Baurate-----------------------------------------------------------------
0   9600
1   19200
2   38400
3   57600
4   115200
5   230400
--------------------------------------------------------------------------------
*/
#define DARK_ON 1
#define LIGHT_ON 0
#define COM 2
#define DAT 1
//definitions
#ifdef LSA08_USE_UART
    #ifndef BAUDRATE
        #define BAUDRATE 9600
    #endif

    #ifndef LSA08_UE
        //#error "Pls define LSA08_UE pin"
        #warning "use LSA08 UART command for line retrieval instead of stream"
    #endif

    #ifndef LSA08_UE_TRIS
        //#error "Pls define LSA08_UE_TRIS"
         #warning "use LSA08 UART command for line retrieval"
    #endif

    #ifndef _XTAL_FREQ
        #error "Pls define _XTAL_FREQ"
    #endif

    #ifndef LSA08_UARTMODE
        #define LSA08_UARTMODE 0x00
    #endif

//    #ifndef LSA08_ADD
//        #define LSA08_ADD 0x01
//    #endif

    #if defined(LSA08_USE_UART)
        #warning "LSA08_USE_UART"
    #elif defined (LSA08_USE_ANALOG)
        #warning "LSA08_USE_ANALOG"
    #else
        #define LSA08_USE_UART
        #warning "default to LSA08_USE_UART"
    #endif

#elif defined (LSA08_USE_ANALOG)
#ifndef ANALOG_MIDVAL
    #define ANALOG_MIDVAL=120
    #warning "Pls define ANALOG_MIDVAL: ANALOG_MIDVAL default to 120"
#endif

#else
    #error "Pls define LSA08_USE_ANALOG  or LSA08_USE_UART"
#endif

//variable
unsigned char rec[2];
extern unsigned char LSA08_ADD;
//function prototypes
//internal
extern void ADC_INIT(void);
extern unsigned short GET_ADC(void);
extern void UART_INIT(void);
extern void UART_SEND(char data);
extern unsigned char UART_REC(void);
extern void UART_DUMP(void);
extern unsigned char ERR_FLAG;
char UART_RESPONSE(unsigned char rec_bytes);
unsigned char SEND_PACKET(unsigned char, unsigned char command, unsigned char value,unsigned char com_dat);



#ifdef LSA08_USE_ANALOG

char LSA08_Init(void){

    ADC_INIT();
    return(1);

}

signed char LSA08_GetPosition(void){
    unsigned short position;
    signed char pos;
    position= GET_ADC();
    //position+=GET_ADC();
    //position=position>>1;   //averaging 2 values
    position=position>>2;   //8bits value

    if (position >245) return(127);

    pos=(signed char)position-(signed char)ANALOG_MIDVAL;
    return(pos);
}

#elif defined (LSA08_USE_UART)

char UART_RESPONSE(unsigned char rec_bytes)
{

    rec[0]=UART_REC();
    if(rec_bytes==DAT)
        return;

    rec[1]=UART_REC();
    
    
    if (rec[0]== 'O') {
        if (rec[1] == 'K') {
            return (1);
         } else
        return (0);
    }
    else
        return(0);
}

unsigned char SEND_PACKET(unsigned char add, unsigned char command, unsigned char value, unsigned char com_dat)
{
    UART_SEND(add);		//header
    UART_SEND(command);		//contrast
    UART_SEND(value);		//data value
    UART_SEND(add+command+value);		//check sum
    return (UART_RESPONSE(com_dat));
}

char LSA08_Init(void){

    UART_INIT();   
#ifdef  LSA08_UE
    LSA08_UE=1;
#endif
    LSA08_ClearJunction();
    return(LSA08_UARTMode(LSA08_UARTMODE));

}



#if (LSA08_UARTMODE==1)

#ifdef LSA08_UE
unsigned char LSA08_GetSensor(void){
    unsigned char sensorbin;

    LSA08_UE=0;
    do{
    sensorbin= UART_REC();       //line post value from 0 to 70
    }while(ERR_FLAG);
    LSA08_UE=1;
    UART_DUMP();
    return(sensorbin);

}
#else
unsigned char LSA08_GetSensor(void){

    SEND_PACKET(LSA08_ADD, 'P', 1, DAT); //get the sensor digital value
    return(rec[0]);     // get the sensor value from the buffer
}
#endif

#elif (LSA08_UARTMODE==2)

#ifdef LSA08_UE
signed char LSA08_GetPosition(void){

    unsigned char linepost;
    signed char post;
    LSA08_UE=0;
    do{
        linepost= UART_REC();       //line post value from 0 to 70
    }while(ERR_FLAG);
    LSA08_UE=1;

    UART_DUMP();

    if(linepost==255) return(127);  //no line

    post=(signed char)linepost-35;   //offset so that center is 0

    return(post);
}
#else
signed char LSA08_GetPosition(void){
    unsigned char linepost;
    signed char post;
    SEND_PACKET(LSA08_ADD, 'P', 2,DAT); //get the sensor digital value
    linepost=rec[0];

    if(linepost==255) return(127);  //no line
    post=(signed char)linepost-35;   //offset so that center is 0
    return(post);
}
#endif
#endif

unsigned char LSA08_GetJunction(void){

    SEND_PACKET(LSA08_ADD, 'X', 1,DAT); //get the junction count
    return(rec[0]);     // get the juction count from the buffer

}

char LSA08_ClearJunction(void){

    return(SEND_PACKET(LSA08_ADD, 'X', 0,COM)); //clear junction count register

}

char LSA08_Calibrate(void){

    return(SEND_PACKET(LSA08_ADD, 'C', 0,COM));
}

char LSA08_LineMode(unsigned char mode){

    return(SEND_PACKET(LSA08_ADD, 'L', mode,COM));
}

char LSA08_Threshold(unsigned char threshold){

    if(threshold>7) threshold=7; 
    return(SEND_PACKET(LSA08_ADD, 'T', threshold,COM));
}

char LSA08_JunctionWidth(unsigned char jw){

    if(jw>8) jw=8;
    if(jw==0) jw=1;
    return(SEND_PACKET(LSA08_ADD, 'J', jw,COM));
}

char LSA08_UARTMode(unsigned char mode){

    if(mode>3) mode=0;
    return(SEND_PACKET(LSA08_ADD, 'D', mode,COM));
}

char LSA08_BL(unsigned char BL){

    if(BL>10) BL=10;
    return(SEND_PACKET(LSA08_ADD, 'B', BL,COM));
}

char LSA08_Contrast(unsigned char contrast){

    return(SEND_PACKET(LSA08_ADD, 'S', contrast,COM));
}
#endif
