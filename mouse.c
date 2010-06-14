//#define PIC32_STARTER_KIT

#include <p32xxxx.h>
#include <plib.h>
#include <stdint.h>
//#include "Micromouse.h"
#include "Movement.h"
/*
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
#pragma config DEBUG    = ON			// Debugger Enable/Disable
    
#endif
*/


volatile	int		fUpdLeds;
void SerialInit();
void Stop(void);


    unsigned int countRight, countLeft;
    unsigned int oldLeft, newLeft;
    unsigned int oldRight, newRight;



void Forward(int left, int right)
{
        
    char concat[5] = "\x8C\x00\x88\x00";
    int j = 0;
    
    #ifdef DEBUGGING_ON
    DBPRINTF("Driving Forward.\n");
    #endif
    
    concat[1] = right;
    concat[3] = left;
    txput(concat);
   
    delay(forward_delay);    

    Stop();
        
    return;
  
}

/*
void Stop(void)
{
    #ifdef DEBUGGING_ON
    DBPRINTF("Stopping.\n");
    #endif
    
    txput(motors_stop);
    delay(4000);
    return;
}

void TurnLeft(void)
{
    #ifdef DEBUGGING_ON
    DBPRINTF("Turning Left.\n");
    #endif
    
    txput(SPINLEFT);  
    delay(spin_delay_l);
    Stop();

	return;
}

void TurnRight(void)
{
    #ifdef DEBUGGING_ON
    DBPRINTF("Turning Right.\n");
    #endif
    
    txput(SPINRIGHT);  
    delay(spin_delay_r);
    Stop();

	return;
}

int encoder_watch(int turns){
    
    #ifdef DEBUGGING_ON
    DBPRINTF("Watching Encoders.\n");
    #endif
    
    oldLeft = 0;
    newLeft = 0;
    oldRight = 0;
    newRight = 0;
    countRight = 0;
    countLeft = 0;

    TRISA |= (1 << 2);
    TRISC |= (1 << 3);

    while ((countLeft < turns) && (countRight < turns)){
        oldRight = newRight;
        oldLeft = newLeft;
        delay(5);
        
        newRight = (PORTA & (1 << 2));
        newLeft = (PORTC & (1 << 3));
        
        if (newRight == (1 << 2) && oldRight == 0)
            {countRight += 1;}
    

        if (newLeft == (1 << 3) && oldLeft == 0)
            {countLeft += 1;}

    }

    return (countLeft - countRight);
}
*/



void orientation_correct(){

    /*
    if (getSensorArrayLeft() == 128 || getSensorArrayLeft() == 192 || getSensorArrayRight() == 16){
        //DBPRINTF("Sitting Right.\n");
        
        concat[1] = right + 5;
        concat[3] = left - 5;
        txput(concat);
        
        j = encoder_watch(4);
        Stop();
 
        return;
        
    }
    else if (getSensorArrayRight() == 128 ||  getSensorArrayRight() == 192  || getSensorArrayLeft() ==  16){
        //sitting left of center
        //DBPRINTF("Sitting Left.\n");
        
        concat[1] = right - 5;
        concat[3] = left + 5;
        txput(concat);
        
        j = encoder_watch(4);
        Stop();
        
        return;   
    }*/
	int leftCapture = getSensorArrayLeft();
	int rightCapture = getSensorArrayRight();
	if (leftCapture == 32) {return;} //perfect
	else if (rightCapture == 32) {return;} //perfect
	else if ((leftCapture <= 24) || (rightCapture == 128)) { //sitting left of center
		
	} else if ((rightCapture <= 24) || (leftCapture == 128)) { //sitting right of center

	} else {return;}

return;
}



int main(void)
{
	int j = 0;
	char command[5] = "\x88\x20\x8C\x20";
	
	//#ifdef DEBUGGING_ON
	DBINIT();
	DBPRINTF("Hello World!\n");
	//#endif

	
	SerialInit();
	ArraysInit();

    delay(10000);

    //Forward(lspeed, rspeed);
    //Stop();
	   
    //TurnLeft();
	
	//TurnRight();
    


    
    return 0;
}


