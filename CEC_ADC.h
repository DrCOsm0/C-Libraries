///////////////////////////////////////////////////////////
//
// ADC Functions to read analog voltages
//
////////////////////////////////////////////////////////////
#define CEC_ADC

#ifndef CEC_DEFINES
#define CEC_DEFINES
#include "CEC_defines.h"
#endif

// This contains a number of prewritten functions
// to make setup and use of analog voltage functions easier

// The ADC has single ended and differential modes.
// However, Differential mode is not guaranteed
// in DIP packages so only single ended is done here

// Set the reference voltage.  There are three options
// 5 volt, 2.56 volt and external.  If an external
// reference is supplied, the internal options 
// must not be used. 

// The three reference voltages
// One of these is the "Ref" arg in ADC_Init()
// and ADC_SetReference()
#define ADC_REF_500V (1 << REFS0)                 // 5 volt supply
#define ADC_REF_256V ((1 << REFS0)|(1 << REFS1))  // 2.56 volt internal reference
#define ADC_REF_EXTV 0                            // External reference at AREF pin     

// 8-bit or 10-bit mode
// One of these is the "Bits" arg in ADC_Init()
// and ADC_SetPrecision()
#define ADC_8_BIT (1 << ADLAR)
#define ADC_10_BIT 0


void ADC_SetReference(BYTE Ref)
{
 // The reference is set by 2 bits, REFS0 and REFS1
 // in ADMUX.  The defines are set to serve as a mask
 // for these bits 

 // Clear the reference bits
 ADMUX &= ~((1 << REFS0)|(1 << REFS1));

 // Set the requested bits
 ADMUX |= Ref;

 // Might as well set the ADC clock speed as well.
 // speeds between 50kHz and 200 kHz are needed
 // At 8 MHz and with 2^n divisors available
 // this is divide by 64
 ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(0 << ADPS0);
 // To figure the resulting divide by, in the ADPSm
 // bits, m is the binary bit to create the exponent for 2^n
 // For example, setting ADPS 2 and 1 gioves an exponent of 0b110
 // or 6 for a divide by of 2^6 = 64.  Setting just ADPS0 would be
 // 0b001 for a divide by of 2^1 = 2

}

// The ADC can operate in 8-bit or 10-bit mode
// The Bits argument here is either 
// ADC_8_BIT or ADC_10_BIT
void ADC_SetPrecision(BYTE Bits)
{
 // Clear the reference bits
 ADMUX &= ~(1 << ADLAR);

 // Set the requested bits
 ADMUX |= Bits;
}

// Might as well put the two functions into a single with two args
void ADC_Init(BYTE Ref, BYTE Bits)
{
 ADC_SetReference(Ref);
 ADC_SetPrecision(Bits);
}

// The right 3 bits of ADMUX pick the pin to read
// There are two read routines. one for 10-bit
// that returns a USHORT and one for 8-bit that
// returns a BYTE
BYTE ADC_Read8Bit(BYTE Pin)
{
 ADMUX &= 0b11100000;              // Clear the MUX bits
 ADMUX |= Pin;                     // Set the MUX pin
 SET_BIT(ADCSRA, ADEN);            // Enable the ADC
 SET_BIT(ADCSRA, ADSC);            // Set start conversion bit
 while(TEST_BIT(ADCSRA, ADSC));    // Wait until its done by watching the Start Conversion bit
 return ADCH;                      // Return the left justified high 8 bits
}

USHORT ADC_Read10Bit(BYTE Pin)
{
 USHORT V;

 ADMUX &= 0b11100000;              // Clear the MUX bits
 ADMUX |= Pin;                     // Set the MUX pin
 SET_BIT(ADCSRA, ADEN);            // Enable the ADC
 SET_BIT(ADCSRA, ADSC);            // Set start conversion bit
 while(TEST_BIT(ADCSRA, ADSC));    // Wait until its done by watching the Start Conversion bit
 V = ADCL;                         // Read the lower 8 bits
 V += ADCH << 8;                   // Read and shift the upper two bits
 return V;
}


