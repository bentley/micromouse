#include "Micromouse.h"


#define MAZECENTER  0X77
#define NORTH       0x00
#define SOUTH       0x01
#define EAST        0x02
#define WEST        0x03
#define FALSE       0
#define TRUE        1

unsigned int myPosition = 0x00;
unsigned int myOrientation = NORTH;


//Movement.h



void TurnRight(){
	//put other code right below here
    myPosition = myPosition;
    txput(SPINRIGHT);  
    delay(spin_delay_r);
    Stop();
	switch(myOrientation){
		case NORTH: myOrientation = EAST; break;
		case SOUTH: myOrientation = WEST; break;
		case EAST:	myOrientation = SOUTH; break;
		case WEST:	myOrientation = NORTH; break;
		default: myOrientation = myOrientation; break;
	}	
	delay(1000);
	forward_correct();
return;
}


void TurnLeft(){
	//put other code right below here
    myPosition = myPosition;
    txput(SPINLEFT);  
    delay(spin_delay_l);
    Stop();

	switch(myOrientation){
		case NORTH: myOrientation = WEST; break;
		case SOUTH: myOrientation = EAST; break;
		case EAST:	myOrientation = NORTH; break;
		case WEST:	myOrientation = SOUTH; break;
		default: myOrientation = myOrientation; break;
	}
	delay(1000);
	forward_correct();
return;
}


void Forward(left, right){

	//put other code right below here

    char command[5] =  motors_forward;
    int j = 0;
    
    command[1] = right;
    command[3] = left;
    txput(command);
   
    delay(forward_delay);    

    Stop();

	myOrientation = myOrientation;
	
	switch(myOrientation){
		case NORTH: myPosition += 0x10; break;
		case SOUTH:	myPosition -= 0x10; break;
		case EAST:	myPosition += 0x01; break;
		case WEST:	myPosition -= 0x01; break;
		default: myPosition = myPosition;
	}	
	delay(1000);
return;
}

void Stop(void)
{   
    txput(motors_stop);
    delay(4000);
    return;
}
