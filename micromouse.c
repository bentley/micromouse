#define PIC32_STARTER_KIT

#include <p32xxxx.h>
#include <stdint.h>

#include "micromouse.h"
#include "movement.h"
#include "serial.h"

void navigateMaze(int ldm);
void choose(int ldm);
void move(int direction);
int* locateWalls(void);

int
main(void)
{
    int rightpos, frontpos;
    initserial();

    delay(10000);

    transmit(4, LFORW, 0x60, RFORW, 0x60);

    for (;;) {
        rightpos = getSensorArrayRight();
        frontpos = getSensorArrayFront();

        if (rightpos == 0) { // no wall to the right
            transmit(4, LFORW, 0x30, RFORW, 0x30); // go to the middle of the cell
            delay(400);

            transmit(4, LFORW, 0x30, RBACKW, 0x30); // turn right 90 degrees
            delay(800);

            transmit(4, LFORW, 0x30, RFORW, 0x30); // go forward one cell
            delay(400);
        } else if (!(frontpos & 0xF)) { // room to go forward
            if (rightpos == 32 || rightpos == 0)
                transmit(4, LFORW, 0x30, RFORW, 0x30); // right on target
            else if (rightpos & 16)
                transmit(4, LFORW, 0, RFORW, 0x30); // too far right
            else
                transmit(4, LFORW, 0x30, RFORW, 0); // too far left
        } else { // a wall in the way
            transmit(4, LBACKW, 0x30, RFORW, 0x30); // turn left 90 degrees
            delay(800);
            stop();
        }
    }

    return 0;
}


void
delay(int time)
{
    int i;
    for (i = 0; i < 100 * time; ++i)
        ; // do nothing
    
    return;
}
