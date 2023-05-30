
#ifndef AVR_H
#define AVR_H

#define BYTE   unsigned char
#define USHORT unsigned short
#define ULONG  unsigned long

#define SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#endif