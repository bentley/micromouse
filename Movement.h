#ifndef	_MOVEMENT_H_
#define	_MOVEMENT_H_

#include "micromouse.h"
#include "serial.h"


#define MAZECENTER  0X77
#define NORTH       0x00
#define SOUTH       0x01
#define EAST        0x02
#define WEST        0x03

unsigned int myPosition = 0x00;
unsigned int myOrientation = NORTH;

void TurnRight(){
	//put other code right below here
    myPosition = myPosition;
    	transmit(4, LFORW, 0x80, RBACKW, 0x80); // spin right
    delay(spin_delay_r);
    stop();
	switch(myOrientation){
		case NORTH: myOrientation = EAST; break;
		case SOUTH: myOrientation = WEST; break;
		case EAST:	myOrientation = SOUTH; break;
		case WEST:	myOrientation = NORTH; break;
		default: myOrientation = myOrientation; break;
	}	
	delay(1000);
	forward_correct();
}


void TurnLeft(){
    myPosition = myPosition;
	    transmit(4, LBACKW, 0x80, RFORW, 0x80); // spin left
    delay(spin_delay_l);
    stop();

	switch(myOrientation){
		case NORTH: myOrientation = WEST; break;
		case SOUTH: myOrientation = EAST; break;
		case EAST:	myOrientation = NORTH; break;
		case WEST:	myOrientation = SOUTH; break;
		default: myOrientation = myOrientation; break;
	}
	delay(1000);
	forward_correct();
}


void Forward(left, right){
    transmit(4, LFORW, left, RFORW, right);
    
	myOrientation = myOrientation;
	
	switch(myOrientation){
		case NORTH: myPosition += 0x10; break;
		case SOUTH:	myPosition -= 0x10; break;
		case EAST:	myPosition += 0x01; break;
		case WEST:	myPosition -= 0x01; break;
		default: myPosition = myPosition;
	}
return;
}

void stop(void)
{   
    transmit(4, LFORW, 0, RFORW, 0); // stop the motors
}
#endif
