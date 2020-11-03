//////////////////////////////////////////////////////
//
// #defines for most CEC projects. 
//
/////////////////////////////////////////////////////
#define CEC_DEFINES
// Define the CPU speed of the chip.  This can
// be set by programming fuses.  Whatever is set
// in the fuses should be entered here.
#define F_CPU 8000000UL 
// This line just allows the built in delay functions
// to use a variable calling argument (like _delay_us(x);)
// instead of requiring a constant (like _delay_us(500);)
#define __DELAY_BACKWARD_COMPATIBLE__ 

// All AVR projects need to include avr/io.h.  This
// loads all the register names (like PORTB) for the
// specific chip used.
#include <avr/io.h>

// This include is only neeed to use the built-in
// microsecond (_delay_us()) and millisecond
// (_delay_ms()) functions 
#include <util/delay.h>

// This include is needed only if interrupt service
// routines (ISR) are used.  Many AVR functions like
// UART and PWM use ISRs
#include <avr/interrupt.h>

// Some simple macros to test, set, clear, and flip individual
// bits in a register.  Reg is the register name like
// PORTA or USCRB, Bit is the bit position from 0 to 7
#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

// AVRs frequently use unsigned variable.  You can speify
// these by typing unsigned short for 16 bit, unsigned
// long for 32 and unsigned char for 8 bit.  I use simple
// defines to avoid all the typing
#define BYTE   unsigned char
#define USHORT unsigned short
#define ULONG  unsigned long

// Basic macros
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x)    ((x) <  0  ? (-(x)) : (x))

#define NEXT(x, Range) (((x) + 1) % (Range))
#define PREV(x, Range) (((x) + ((Range) - 1)) % (Range))

// Define a single clock cycle delay
#define NOP  __asm__ __volatile__("nop")
