#define PIC32_STARTER_KIT

#include <p32xxxx.h>
#include <stdarg.h>

#include "serial.h"

void
transmit(int count, ...)
{
    // send _count_ bytes to the motor controller

    va_list ap;
    int i;

    va_start(ap, count);
    for (i = 0; i < count; ++i) {
        while (U2STA & (1 << 9))
            ; // wait for room in the buffer

        U2TXREG = va_arg(ap, int);
    }
    
    va_end(ap);
}

void
initserial(void)
{
    // Enable transmission (PIC32 ref. manual, 21.5.3)
    U2MODE  = 0; // PDSEL: 8-bit data, no parity
    U2BRG   = 33000000L / 16 / 38400 - 1;  // baud rate (38400)

    // Enable UART (PIC32 ref. manual, 21.4.1)
    U2STA  = (1 << 12) | (1 << 10); // enable RX, TX
    U2MODE |= (1 << 15);            // enable UART2
    delay(1000);
    transmit(1, 0x65);
}
