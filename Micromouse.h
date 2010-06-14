//#define DEBUGGING_ON

#define PIC32_STARTER_KIT

#include <p32xxxx.h>
#include <plib.h>
#include <stdint.h>

//Timer
#define FOSC 60E6
#define PB_DIV 8
#define PRESCALE 256
#define MSEC 10E-3
#define T1_TICK (500 * MSEC * FOSC)/(PB_DIV * PRESCALE)

//Motor
#define rspeed 0x24
#define lspeed 0x20

#define rspeedslow 0x10  //adjust these
#define lspeedslow 0x10		//adjust these

#define forward_delay 4300  //2750 usb
#define spin_delay_l 2550     //1800 usb
#define spin_delay_r 2400

#define spin_correction_delay 1300

#define motors_forward "\x8C\x00\x88\x00"  //right wheel, left wheel
#define motors_backward "\x8A\x00\x8E\x00" 

#define motors_stop "\x88\x01\x8C\x01"
#define ticks 10
#define SPINRIGHT "\x88\x20\x8E\x20"
#define SPINLEFT "\x8A\x20\x8C\x20"


//functions
void txput(char *str);
void delay(int time);
void ArraysInit();
int getSensorArrayRight();
int getSensorArrayLeft();
int getSensorArrayFront();
void SerialInit();
void forward_correct();
void orientation_correct();
//movement functions
void TurnRight();
void TurnLeft();
void Forward();
void Stop(void);




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

void txput(char *str)
{
    int i;
    for (i = 0; i < strlen(str); ++i) {
        while (U2STA & (1 << 9))
            ; // check for room in buffer
    
        U2TXREG = str[i];
    }
    return;
}

inline void delay(int time)
{
    int i;
    for (i = 0; i < 100 * time; ++i)
        ; // do nothing
    
    return;
}


void ArraysInit(){
    
    AD1PCFG = 0xFFFF; // Set PORTB <0-7> to digital
}


int getSensorArrayLeft()
{
    int CaptureB;
    int Array = 0x0000;

    TRISB &= ~((1 << 1) | (1 << 2) | (1 << 3) | (1 << 0)); //set bits <0,1,2,3> outputs of RB

    PORTB |= ((1 << 1) | (1 << 2) | (1 << 3) | (1 << 0));           
   
    delay(1000);                        // wait for cap to charge
 
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
    int CaptureD, CaptureC;
    int Array = 0x0000;

    TRISD &= ~((1 << 9) | (1 << 7) | (1 << 1)); //set bits <9,7,1> to outputs of RD
    TRISC &= ~(1 << 1);  //set bit <1> to output of RC
    
    PORTD |= ((1 << 9) | (1 << 7) | (1 << 1));
    PORTC |= (1 << 1);
           
    delay(1000);                        // wait for cap to charge
 
    TRISD = 0xFFFF;         // Set PORTD <all> as input
    TRISC = 0xFFFF;         // Set PORTC <all> as input
    
    delay(5);  // Wait a while for the walls to be detected
    
    
    CaptureD = PORTD;
    CaptureC = PORTC;
    
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
    int CaptureE;
    int Array = 0x0000;

    TRISE &= ~(0xFF); //~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5)); //set bits <0,1,2,3> to outputs of RE

    PORTE |= (0xFF); //((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5));
           
    delay(1000);                        // wait for cap to charge
 
    TRISE = 0xFFFF;         // Set PORTE <all> as input
    
    delay(5);  // Wait a while for the walls to be detected
    
    
    CaptureE = PORTE;
    
    if ((PORTE & 0x0008) == 0x0000){ //RE3
        Array |= 0x0080; //Bit 7
    }
    if ((PORTE & 0x0004) == 0x0000){ //RE2
        Array |= 0x0040;  //Bit 6
    }
    if ((PORTE & 0x0002) == 0x0000){ //RE1
        Array |= 0x0020; //Bit 5
    }
    if ((PORTE & 0x0001) == 0x0000){ //RE0
        Array |= 0x0010; //Bit 4 
    }
    if ((PORTE & 0x0010) == 0x0000){ //RE4
        Array |= 0x0008; //Bit 3
    }
    if ((PORTE & 0x0020) == 0x0000){ //RE5
        Array |= 0x0004;  //Bit 2
    }
	if ((PORTE & 0x0040) == 0x0000){ //RE6
        Array |= 0x0002;  //Bit 1
    }
   
   //DBPRINTF("Front Array: %i\n", Array); 
   return Array;
}


void SerialInit(){
    // Enable transmission (21.5.3)
    U2MODE  = 0; // PDSEL: 8-bit data, no parity
    U2BRG   = 33000000L / 16 / 38400 - 1;  // baud rate (38400)
    
    // Enable UART (21.4.1)
    U2STA  = (1 << 12) | (1 << 10); // enable RX, TX
    U2MODE |= (1 << 15);             // enable UART2
    delay(1000);
    txput("\x65");
}


void forward_correct(){

	int arrayCapture = getSensorArrayFront();

	if (arrayCapture == 0){return;}
	else if (arrayCapture == 8){return;}
	else if ((arrayCapture > 0) && (arrayCapture < 8)) {
			char command[5] = motors_backward;
			command[1] = rspeedslow; //right wheel speed decreased
			command[3] = lspeedslow;	//left wheel speed decreased
			txput(command);
			forward_correct(); 
			Stop();
			return;
	} else if (arrayCapture > 8) {
			char command[5] = motors_forward;
			command[1] = rspeedslow; //right wheel speed decreased
			command[3] = lspeedslow;	//left wheel speed decreased
			txput(command);
			forward_correct(); 
			Stop();
			return;
	} else {return;}

return;
}



void orientation_correct(){
	int leftCapture = getSensorArrayLeft();
	int rightCapture = getSensorArrayRight();
	
	if (((leftCapture > 0) && (leftCapture <= 24)) || (rightCapture >= 128)) { //sitting left of center
		char command[5] = SPINRIGHT;
		command[1] = rspeedslow; //right wheel speed decreased
		command[3] = lspeedslow;
		txput(command);
		delay(spin_correction_delay);
		Stop();	
	} else if (((rightCapture > 0) && (rightCapture <= 24)) || (leftCapture >= 128)) { //sitting right of center
		char command[5] = SPINLEFT;
		command[1] = rspeedslow; //right wheel speed decreased
		command[3] = lspeedslow;
		txput(command);
		delay(spin_correction_delay);
		Stop();
	} else if (leftCapture == 32) {return;
	} else if (rightCapture == 32) {return;
	} else {return;}

return;
}
