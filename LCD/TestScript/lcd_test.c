
#define F_CPU 8000000UL // 16 MHz clock speed



#define D4 eS_PORTC2
#define D5 eS_PORTC3
#define D6 eS_PORTC4
#define D7 eS_PORTC5
#define RS eS_PORTC0
#define EN eS_PORTC1
#include "lcd.h"
#include <stdlib.h>

int buff = 3;
int main(void)
{

   DDRC = 0xFF;
   Lcd4_Init();
   Lcd4_Clear();

   Lcd4_Set_Cursor(1,2);

   char buffer[50];
   char a = 5;

  	itoa(a,buffer,10);
	  
	Lcd4_Write_String(buffer);

   
}
