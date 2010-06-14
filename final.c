//#include "Micromouse.h"

#include "Movement.h"




void navigateMaze(int ldm);
void choose(int ldm);
void move(int direction);
int* locateWalls(void);




void navigateMaze(int ldm){
	//correct straight
	//correct Forward
	//move!!!

	orientation_correct();
	choose(ldm);
	return;
}

void choose(int ldm){

int walls[4] = {0,0,0,0};
	switch(myOrientation){
        case NORTH:  
                    if(getSensorArrayRight() == 0) {walls[EAST] = 0;}
                    else {walls[EAST] = 1;}
                    
                    if(getSensorArrayFront() == 0) {walls[NORTH] = 0;}
                    else {walls[NORTH] = 1;}
                    
                    if(getSensorArrayLeft() == 0) {walls[WEST] = 0;}
                    else {walls[WEST] = 1;}
					break;

        case SOUTH:
                    if(getSensorArrayRight() != 0x00) {walls[WEST] = 1;}
                    else {walls[WEST] = 0;}
                    
                    if(getSensorArrayFront() != 0x00) {walls[SOUTH] = 1;}
                    else {walls[SOUTH] = 0;}
                    
                    if(getSensorArrayLeft() != 0x00) {walls[EAST] = 1;}
                    else {walls[EAST] = 0;}
                   	
					break;
        			
		case EAST:
                    if(getSensorArrayRight() != 0x00) {walls[SOUTH] = 1;}
                    else {walls[SOUTH] = 0;}
                    
                    if(getSensorArrayFront() != 0x00) {walls[EAST] = 1;}
                    else {walls[EAST] = 0;}
                    
                    if(getSensorArrayLeft() != 0x00) {walls[NORTH] = 1;}
                    else {walls[NORTH] = 0;} 
                    
					break;

        case WEST:
                    if(getSensorArrayRight() != 0x00) {walls[NORTH] = 1;}
                    else {walls[NORTH] = 0;}
                    
                    if(getSensorArrayFront() != 0x00) {walls[WEST] = 1;}
                    else {walls[WEST] = 0;}
                    
                    if(getSensorArrayLeft() != 0x00) {walls[SOUTH] = 1;}
                    else {walls[SOUTH] = 0;}
          			
					break;

        default:
            break;
          
    
      }

	forward_correct();
    
    
    if((myPosition == 0x77) || (myPosition == 0x78) || (myPosition == 0x87) || (myPosition == 0x88)){
		//maze center located!!!!
		while(1) {txput(SPINLEFT);}
    }
    
    if ((ldm != SOUTH) && (walls[NORTH] == 0)){
        move(NORTH);
        navigateMaze(NORTH);
    }
    
    if ((ldm != WEST) && (walls[EAST] == 0)){
        move(EAST);
        navigateMaze(EAST);
    }
    
    if ((ldm != EAST) && (walls[WEST] == 0)){
        move(WEST);
        navigateMaze(WEST);
    }
    
    if ((ldm != NORTH) && (walls[SOUTH] == 0)){
        move(SOUTH);
        navigateMaze(SOUTH);
    }

	//dead end, return one square
	switch(ldm){
		case NORTH: move(SOUTH); break;
		case SOUTH: move(NORTH); break;
		case WEST: move(EAST); break;
		case EAST: move(WEST); break;
		default: break;
	}	
	
	return;

}


void move(int direction){

    switch(direction){
        case NORTH: 
			if(myOrientation == NORTH) {Forward(lspeed, rspeed);}
            else if(myOrientation == SOUTH){TurnLeft(); TurnLeft(); orientation_correct(); Forward(lspeed,rspeed);}
            else if(myOrientation == EAST){TurnLeft(); Forward(lspeed,rspeed);}
            else if(myOrientation == WEST){TurnRight(); Forward(lspeed,rspeed);}
            else{return;}
			break; 

		case SOUTH:
            if(myOrientation == NORTH){TurnLeft(); TurnLeft(); orientation_correct(); Forward(lspeed,rspeed);}
            else if(myOrientation == SOUTH){Forward(lspeed,rspeed);}
            else if(myOrientation == EAST){TurnRight(); Forward(lspeed,rspeed);}
            else if(myOrientation == WEST){TurnLeft(); Forward(lspeed,rspeed);}
            else{return;}	
			break;
            
        case EAST:
            if(myOrientation == NORTH){TurnRight(); Forward(lspeed,rspeed);}
            else if(myOrientation == SOUTH){TurnLeft(); Forward(lspeed,rspeed);}
            else if(myOrientation == EAST){Forward(lspeed,rspeed);}
            else if(myOrientation == WEST){TurnLeft(); TurnLeft(); orientation_correct(); Forward(lspeed,rspeed);}
            else{return;}
			break;
            
        case WEST:
            if(myOrientation == NORTH){TurnLeft(); Forward(lspeed,rspeed);}
            else if(myOrientation == SOUTH){TurnRight(); Forward(lspeed,rspeed);}
            else if(myOrientation == EAST){TurnLeft(); TurnLeft(); orientation_correct(); Forward(lspeed,rspeed);}
            else if(myOrientation == WEST){Forward(lspeed,rspeed);}
            else{return;}
			break;

    
        default: 
            //DBPRINTF("Error Switch/Case.\n");
			break;
    }
	
	orientation_correct();
	return;
}

/*
int* locateWalls(void){
    int walls[4] = {0,0,0,0};
	switch(myOrientation){
        case NORTH:  
                    if(getSensorArrayRight() != 0x00) {walls[EAST] = 1;}
                    else {walls[EAST] = 0;}
                    
                    if(getSensorArrayFront() != 0x00) {walls[NORTH] = 1;}
                    else {walls[NORTH] = 0;}
                    
                    if(getSensorArrayLeft() != 0x00) {walls[WEST] = 1;}
                    else {walls[WEST] = 0;}
					break;

        case SOUTH:
                    if(getSensorArrayRight() != 0x00) {walls[WEST] = 1;}
                    else {walls[WEST] = 0;}
                    
                    if(getSensorArrayFront() != 0x00) {walls[SOUTH] = 1;}
                    else {walls[SOUTH] = 0;}
                    
                    if(getSensorArrayLeft() != 0x00) {walls[EAST] = 1;}
                    else {walls[EAST] = 0;}
                   	
					break;
        			
		case EAST:
                    if(getSensorArrayRight() != 0x00) {walls[SOUTH] = 1;}
                    else {walls[SOUTH] = 0;}
                    
                    if(getSensorArrayFront() != 0x00) {walls[EAST] = 1;}
                    else {walls[EAST] = 0;}
                    
                    if(getSensorArrayLeft() != 0x00) {walls[NORTH] = 1;}
                    else {walls[NORTH] = 0;} 
                    
					break;

        case WEST:
                    if(getSensorArrayRight() != 0x00) {walls[NORTH] = 1;}
                    else {walls[NORTH] = 0;}
                    
                    if(getSensorArrayFront() != 0x00) {walls[WEST] = 1;}
                    else {walls[WEST] = 0;}
                    
                    if(getSensorArrayLeft() != 0x00) {walls[SOUTH] = 1;}
                    else {walls[SOUTH] = 0;}
          			
					break;

        default:
            break;
          
    
      }

return walls;
}
*/

int main(void){
SerialInit();
ArraysInit();
Stop();

delay(10000);

//choose(NORTH);
//navigateMaze(NORTH);

navigateMaze(NORTH);

return 0;	
}
