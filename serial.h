#ifndef	_SERIAL_H_
#define	_SERIAL_H_

// Motor controller commands;
// See Qik 2s9v1 User's Guide, chapter 5
#define LFORW  0x88
#define LBACKW 0x8A
#define RFORW  0x8C
#define RBACKW 0x8E

void transmit(int, ...);
void initserial(void);

#endif
