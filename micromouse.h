#ifndef	_MICROMOUSE_H_
#define	_MICROMOUSE_H_

#include "serial.h"

void delay(int time);

//Timer
#define FOSC 60E6
#define PB_DIV 8
#define PRESCALE 256
#define MSEC 10E-3
#define T1_TICK (500 * MSEC * FOSC)/(PB_DIV * PRESCALE)

//Motor
#define rspeed 0x30
#define lspeed 0x30

#define rspeedslow 0x10  //adjust these
#define lspeedslow 0x10		//adjust these

#define forward_delay 4300  //2750 usb
#define spin_delay_l 2550     //1800 usb
#define spin_delay_r 2400

#define spin_correction_delay 1300

//functions
void ArraysInit();
int getSensorArrayRight();
int getSensorArrayLeft();
int getSensorArrayFront();
void forward_correct();
void orientation_correct();
//movement functions
void TurnRight();
void TurnLeft();
void Forward();
void stop(void);




#ifndef OVERRIDE_CONFIG_BITS

#pragma config ICESEL   = ICS_PGx2		// ICE/ICD Comm Channel Select
#pragma config BWP      = OFF			// Boot Flash Write Protect
#pragma config CP       = OFF			// Code Protect
#pragma config FNOSC    = PRIPLL		// Oscillator Selection
#pragma config FSOSCEN  = OFF			// Secondary Oscillator Enable
#pragma config IESO     = OFF			// Internal/External Switch-over
#pragma config POSCMOD  = HS			// Primary Oscillator
#pragma config OSCIOFNC = OFF			// CLKO Enable
#pragma config FPBDIV   = DIV_2			// Peripheral Clock divisor
#pragma config FCKSM    = CSDCMD		// Clock Switching & Fail Safe Clock Monitor
#pragma config WDTPS    = PS1			// Watchdog Timer Postscale
#pragma config FWDTEN   = OFF			// Watchdog Timer 
#pragma config FPLLIDIV = DIV_2			// PLL Input Divider
#pragma config FPLLMUL  = MUL_16		// PLL Multiplier
#pragma config UPLLIDIV = DIV_2			// USB PLL Input Divider
#pragma config UPLLEN   = OFF			// USB PLL Enabled
#pragma config FPLLODIV = DIV_1			// PLL Output Divider
#pragma config PWP      = OFF			// Program Flash Write Protect
#pragma config DEBUG    = OFF		    // Debugger Enable/Disable
    
#endif

//LED Port Defines
#define IR7_Right 9  //JD3, RD9

void ArraysInit(){
    
    AD1PCFG = 0xFFFF; // Set PORTB <0-7> to digital
}


int getSensorArrayLeft()
{
    int CaptureB;
    int Array = 0x0000;

    TRISB &= ~((1 << 1) | (1 << 2) | (1 << 3) | (1 << 0)); //set bits <0,1,2,3> outputs of RB

    PORTB |= ((1 << 1) | (1 << 2) | (1 << 3) | (1 << 0));           
   
    delay(10);                        // wait for cap to charge
 
    TRISB = 0xFFFF;         // Set PORTB <all> as input

    
    delay(5);  // Wait a while for the walls to be detected
    
    
    CaptureB = PORTB;
    
    if ((PORTB & 0x0008) == 0x0000){ //RB3
        Array |= 0x0010; //IR 5
    }
    if ((PORTB & 0x0004) == 0x0000){ //RB2
        Array |= 0x0020;  //IR 6
    }
    if ((PORTB & 0x0002) == 0x0000){ //RB1
        Array |= 0x0040; //IR 7
    }
    if ((PORTB & 0x0001) == 0x0000){ //RB0
        Array |= 0x0080; //IR 8 
    }
    
//DBPRINTF("Left Array: %i\n", Array);
    return Array;   
}


int getSensorArrayRight()
{
    int Array = 0x0000;

    TRISD &= ~((1 << 9) | (1 << 7) | (1 << 1)); //set bits <9,7,1> to outputs of RD
    TRISC &= ~(1 << 1);  //set bit <1> to output of RC
    
    PORTD |= ((1 << 9) | (1 << 7) | (1 << 1));
    PORTC |= (1 << 1);
           
    delay(10);                        // wait for cap to charge
 
    TRISD = 0xFFFF;         // Set PORTD <all> as input
    TRISC = 0xFFFF;         // Set PORTC <all> as input
    
    delay(5);  // Wait a while for the walls to be detected
    
    if ((PORTD & 0x0200) == 0x0000){ //RD9
        Array |= 0x0040; //Bit 7
    }
    if ((PORTD & 0x0080) == 0x0000){ //RD7
        Array |= 0x0010;  //Bit 5
    }
    if ((PORTD & 0x0002) == 0x0000){ //RD1
        Array |= 0x0020; //Bit 6
    }
    if ((PORTC & 0x0002) == 0x0000){ //RC1
        Array |= 0x0080; //Bit 8 
    }
   
   //DBPRINTF("Right Array: %i\n", Array); 
   return Array;
}


int getSensorArrayFront()
{
    int Array = 0x0000;

    TRISE &= ~(0xFF); //~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5)); //set bits <0,1,2,3> to outputs of RE

    PORTE |= (0xFF); //((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5));
           
    delay(10);                        // wait for cap to charge
 
    TRISE = 0xFFFF;         // Set PORTE <all> as input
    
    delay(5);  // Wait a while for the walls to be detected
    
    if ((PORTE & 0x0008) == 0x0000) //RE3
        Array |= 0x0080; //Bit 7
    if ((PORTE & 0x0004) == 0x0000) //RE2
        Array |= 0x0040;  //Bit 6
    if ((PORTE & 0x0002) == 0x0000) //RE1
        Array |= 0x0020; //Bit 5
    if ((PORTE & 0x0001) == 0x0000) //RE0
        Array |= 0x0010; //Bit 4
    if ((PORTE & 0x0010) == 0x0000) //RE4
        Array |= 0x0008; //Bit 3
    if ((PORTE & 0x0020) == 0x0000) //RE5
        Array |= 0x0004;  //Bit 2
	if ((PORTE & 0x0040) == 0x0000) //RE6
        Array |= 0x0002;  //Bit 1
   
   //DBPRINTF("Front Array: %i\n", Array); 
   return Array;
}


void forward_correct(){

	int arrayCapture = getSensorArrayFront();

	if (arrayCapture == 0 || arrayCapture == 8)
	    return;

	if ((arrayCapture > 0) && (arrayCapture < 8)) {
    	transmit(4, LBACKW, lspeedslow, RBACKW, rspeedslow);
		forward_correct(); 
		stop();
		return;
	}
	
    if (arrayCapture > 8) {
		transmit(4, LFORW, lspeedslow, RFORW, rspeedslow);
		forward_correct(); 
		stop();
		return;
	}

	return;
}



void orientation_correct(){
	int leftCapture = getSensorArrayLeft();
	int rightCapture = getSensorArrayRight();
	
	if (((leftCapture > 0) && (leftCapture <= 24)) || (rightCapture >= 128)) { //sitting left of center
    	transmit(4, LFORW, lspeedslow, RBACKW, rspeedslow); // spin right
		delay(spin_correction_delay);
		stop();	
	} else if (((rightCapture > 0) && (rightCapture <= 24)) || (leftCapture >= 128)) { //sitting right of center
	    transmit(4, LBACKW, lspeedslow, RFORW, rspeedslow); // spin left
		delay(spin_correction_delay);
		stop();
	} else if (leftCapture == 32)
	    return;
	else if (rightCapture == 32)
    	return;
	else
	    return;
}
#endif
