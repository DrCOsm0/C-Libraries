/*
 * test.c
 *
 * Created: 10/27/2020 12:09:16 AM
 * Author : jaibh
 */ 

#define F_CPU 8000000UL 
#include <avr/io.h>
#include <util/delay.h>

#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

void eye_on();
void eye_off();
void candle();
void blue_on();
void blue_off();




int main(void)
{	
	DDRD = 0b11111111;
 

	//PWM OC2
	SET_BIT(TCCR2, WGM20);//sets fast PWM
	SET_BIT(TCCR2, WGM21);
	SET_BIT(TCCR2, COM21);//allows OCR2
	SET_BIT(TCCR2, CS22);//setting 64 prescaler timer 2
 
 
    while (1) 
    {
		
		candle();
		eye_on();
		
		blue_on();
		blue_off();
		blue_on();
		blue_off();
		blue_on();
		blue_off();
		blue_on();
		blue_off();
		
		eye_off();
		_delay_ms(2000);
		
    }
}


void eye_on()
{
	for(int i = 0; i < 255; i++)
	{
		OCR2 = i;
		_delay_ms(20);
	}
}

void eye_off()
{
	for(int i = 255; i > 0; i--)
	{
		OCR2 = i;
		_delay_ms(5);
	}	
}

void candle()
{
	for(int i = 0; i < 100; i++)
	{
		PORTD = 0b00000010;
		_delay_ms(50);
		PORTD = 0b00000011;
		_delay_ms(100);
	}
	PORTD = 0b00000000;
}

void blue_on()
{
	PORTD = 0b01000000;
	_delay_ms(1000);
}

void blue_off()
{
	PORTD = 0b00000000;
	_delay_ms(1000);
}