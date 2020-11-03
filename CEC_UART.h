///////////////////////////////////////////////////////////
//
// UART Functions to do serial communications
//
////////////////////////////////////////////////////////////
#define CEC_UART

#ifndef CEC_DEFINES
#define CEC_DEFINES
#include "CEC_defines.h"
#endif

// A basic serial communication setup library
// This sets up and uses the AVR UART 

// Define a macro to calculate the value needed by the AVR to
// set up for a specified baud rate.
#define BAUD_PRESCALE(Baud) ((F_CPU >> 4) / (Baud) - 1)

// Other specific routines, for example for a binary byte are not really needed
// any such routine can be handled as a macro like:
#define UART_SEND_BINARY_BYTE(b) UART_SendUInt(b, 2, 8)
// For a simple decimal number
#define UART_SEND_DECIMAL_INTEGER(n) UART_SendUInt(n, 10, 1)

// This is a single character external value that holds
// the last byte received over the UART
volatile BYTE UART_Byte;  

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//
// Low level UART routines.  These set up the UART and provide utilities
// to send and receive a data byte
//
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// Initializing the UART
void UART_Init(ULONG BaudRate)
{
 // These two lines come straight from the data sheet, along with
 // the BAUD_PRESCALE macro to convert the baud rate into
 // the UART Baud Rate Registers, High and Low bytes
 UBRRL =  (BYTE)BAUD_PRESCALE(BaudRate);
 UBRRH = (BYTE)(BAUD_PRESCALE(BaudRate) >> 8);
 // Default frame format is 8N1

 // As usual, setting upo the UART requires setting bits in control
 // registers.  For the UART, there are two register, UART
 // Control and Status Register A (UCSRA), B (USCRB) and C 
 // (USCRC) but only B and Ccontrol the setup.  A is used to read 
 // the UART status. Each bit means something and has a name.  I OR 
 // together all 8 bits putting a zero in the ones that aren't 
 // used.  This gives the complete list of options

 // Enable receiver and transmitter and receive complete interrupt 
 UCSRB = (1 << RXCIE)   // The Receive Complete Interrupt Enbale bit
                        // Setting this generates an interrupt when a 
					    // byte is received.  This will cause the ISR
					    // function with argument USART_RXC_vect
					    // to execute   
       | (0 << TXCIE)   // Like RXCIE except to interrupt on transmit
	                    // the ISR argument is USART_TXC_vect
       | (0 << UDRIE)   // UART Data Register Empty Interrupt Enable
	                    // enables the USART_UDRE_vect ISR when no
						// data is in the register
       | (1 << RXEN )   // Receive Enable.  Turns on the UART receive.
	                    // Without this on, You can't receive data
	   | (1 << TXEN )   // Transmit Enable.  Like RXEN but for transmit					 
       | (0 << UCSZ2);  // Used with UCSZ0 and USCZ1 in the C register
	                    // to specify how many bits in a character
						// This bit is only set for AVR to AVR multiple
						// communications which use a special 9-bit mode
						// these remaining two bits of this register are
						// also used for handling 9-bit mode 

/* This entire section for UCSRC is commented out because the default 
   values of the registers are 8 bits, no parity, 1 stop bit.  This is 
   only needed if some other character setting is used      
 UCSRC = (1 << URSEL)   // This bit must be set to write to UCSRC.  
       | (0 << UMSEL)   // Select asynchronous (our usual mode) by
	                    // Setting this to zero.  1 for synchronous mode
       | (0 << UPM1)    // UART Parity Mode (UPM) -takes 2 bits called
	   | (0 << UPM0)    // UPM1 and UPM0, for no parity both are zero, for
	                    // even, UPM1 is set, for odd UPM0 is set
	   | (0 << USBS)    // UART Stop Bit Setting. Clear for 1 stop bit
	                    // set for 2 stop bits
	   | (1 << UCSZ1)	// UART Character Size UCSZ1 and UCSZ0 together 
	   | (1 << UCSZ0);	// determine the bits per character by the formula
                        // 2*UCSZ1 + UCSZ0 + 5.  This means the usual 8-bit
						// has both set.
*/
}

// The ISR is for receiving a character by interrupt.  It puts
// the latest character, in the UART Data Register (UDR), into the external
// variable UART_Byte so it can be read elsewhere in the program.  Since
// our use of the UART in receive mode on the AVR is to read single keystrokes
// this is fine.  If we were streaming data to the AVR we would need to store
// it in an array to hold many bytes instead of just one.  Either way. the 
// AVR holds only a single data byte at any one time in UDR and UDR is used
// both for transmit and receive so sening a byte will overwrite the value 
// just like receiving the next one will.
ISR(USART_RXC_vect) { UART_Byte = UDR; }  //read UART register into value

// A routine to transmit a single byte.  This is the base routine for all that
// follows.
void UART_SendByte(char Data)
{
 // Check the UART Data Register Empty bit (UDRE) in the UART Control
 // and Status Register A (UCSRA).  If this bit is not set, the data register
 // (UDR) is busy so wait until the bit is set 
 while(TEST_BIT(UCSRA, UDRE) == 0);

 // Transmitting the data just means putting it into the data register (UDR)
 // the AVR takes it from there.
 UDR = Data;
}

// Receive a single byte.  This would be used if the ISR were not set to automatically
// receive incoming bytes
unsigned char UART_ReceiveByte()
{
 // Wait until the Receive Complete flag (RXC) in the UART Status and control
 // Register A (USCRA) is set meaning a byte has been received.
 while(!(UCSRA & (1 << RXC)));
 // Just return the value in the UARET Data Register (UDR)
 return UDR;
}

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//
// High level UART routines.  These provide formatting for strings and numbers
// sent over the UART
//
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// This accepts a character array as a pointer and increments the pointer
// allong the array sending the bytes until a NULL terminator is found
void UART_SendString(char *s) 
{ 
 void UART_SendByte(char Byte);

 while(*s != '\0') UART_SendByte(*s++); 
}

// A routine to send a carriage return and linefeed to get
// a new line on the receiving screen.  This just sends out
// the two bytes for carriage return ('\r') then new line
// ('\n') as defined in C.
void UART_CRLF()
{
 void UART_SendByte(char b);

 UART_SendByte('\r');
 UART_SendByte('\n');
}
  

// Now some routines to send out numerical values.

// Rather than write separate routines to convert
// to decimal, octal, hex and binary for each length
// integer,  Write a single routine for any size and
// any base and use this from other specific routines
// Since using bases is limited to unsigned numbers
// Set the argument as a long unsigned.  The output 
// will be truncated for shorter data
// The third argument is how many digits to display
// by adding padding zeros to the left
// A value of zero shows the number with no zero padding 

// From this routine, a bunch of specific Send routines
// are defined as macros in UART.h

void UART_SendUInt(ULONG Num, BYTE Base, BYTE Digits)
{
 void UART_SendString(char *s);

 BYTE Val, Count = 0; // The digit values and digit counter
 
 char b[33]; // Allocate enough memory plus a null byte for the largest data
             // tyoe of 32 bits
 char *c = b + 32; // Set a pointer to the end of the string
 
 *c = '\0'; // Null terminate the string
 
 while(Num || (Count < Digits))
 {
  Val = (BYTE)(Num % Base);   // Figure the next digit
  *(--c) = Val < 10 ? '0' + Val : 'A' + Val - 10; // Put the digit's character in *c
  Num /= Base;                       // Move to the next digit
  Count++;
 }

 UART_SendString(c);  // Send the binary string out
}

// Other specific routines, for example for a binary byte are not really needed
// any such routine can be handled as a macro like:
// #define UART_SEND_BINARY_BYTE(b) UART_SendUInt(b, 2, 8)
// A good place for macros is in UART.h

// This converts a BYTE into a character string of blanks 
// with a mark showing the relative size of Val.  Way to
// the left for Val=0 way to the right for Val=255
// The total resolution is 16 characters

void UART_StripChart(BYTE Val)
{
 char s[18];
 unsigned char i;
 static unsigned char OldVal = 0; 
 char c;

 c = Val == OldVal ? '|' :
     Val  < OldVal ? '/' : 
	                 '\\' ;

 s[17] = '\0';
 s[16] = '|';
 for(i=0; i<16; i++) s[i] = ' ';
 s[Val >> 4] = c;
 UART_SendString(s);
 UART_CRLF();
 OldVal = Val;
}
