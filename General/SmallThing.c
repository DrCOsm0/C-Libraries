#include <CEC_UART.h>
#include <CEC_ADC.h>
#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

#define SERVO_PERIOD 20000
#define DRIVE_MIN    900 
#define DRIVE_MAX    2900
#define DRIVE_MID    1700

#define SENSOR_MIN    1000
#define SENSOR_MAX    2800
#define SENSOR_MID    1700

short sidewall = 0;
short frontwall = 0;
char  i=0;
char  a=0;
short b=0;

void forward();
void stop();

int main(void){
DDRD = 0b11111010;
DDRB = 0b11111111;
DDRC = 0b00000010;
UART_Init(9600);
ADC_Init(ADC_REF_500V, ADC_8_BIT);
sei();

//servo PWM       
SET_BIT(TCCR1B, CS11);//setting 8 prescaler timer 1  
SET_BIT(TCCR1A, WGM11);//setting timer settings to allow ICR1
SET_BIT(TCCR1B, WGM12);
SET_BIT(TCCR1B, WGM13);
SET_BIT(TCCR1A, COM1A1);//setting timer to allow OCR1A
SET_BIT(TCCR1A, COM1B1);//setting timer to allow OCR1B
ICR1 = SERVO_PERIOD;

//Moter PWM
SET_BIT(TCCR0, WGM00);//sets fast PWM
SET_BIT(TCCR0, WGM01);
SET_BIT(TCCR0, COM01);//allows OCR0
SET_BIT(TCCR0, CS00);//setting 64 prescaler timer 2
SET_BIT(TCCR0, CS01);

//interrupt
SET_BIT(MCUCR, ISC00);//enabling any logical change makes an interrupt
SET_BIT(GICR, INT0);//enabling interrupt0 (PD2)

//start
OCR0 = 255;
OCR1A = DRIVE_MID;
OCR1B = SENSOR_MAX;
_delay_ms(10);

while(1){

PORTC = 0b00000010;
sidewall = ADC_Read8Bit(1);
frontwall = ADC_Read8Bit(2);
//UART_SEND_DECIMAL_INTEGER(1234);
//UART_CRLF();


/*if(OCR1B <= SENSOR_MIN){
b = 30;
}

if(OCR1B >= SENSOR_MAX){
b = -30;
}
OCR1B = OCR1B + b;*/

////OCR1B+=20;
//if(OCR1B >= SENSOR_MAX){
//OCR1B = SENSOR_MIN;
//}





if(sidewall <= 150){   //follow left
stop();
OCR1A = 1600;
forward();
_delay_ms(25);
i=0;
}

if(sidewall >= 160){       //follow right  
stop();
OCR1A = 1800;
forward();
_delay_ms(25);
}

if(sidewall >= 200 && i==0){      //hard right turn
forward();
_delay_ms(500);
OCR1A = DRIVE_MAX;
_delay_ms(700);
OCR1A = DRIVE_MID;
_delay_ms(500);
}

if(sidewall <= 25){              //dragging wall correction
stop();
OCR1A = 1300;
forward();
_delay_ms(25);
}

if(frontwall <= 100){      //hard left
stop();
OCR1A = 1200;
forward();
_delay_ms(200);
i=1;
}

if(frontwall > 60){       //default forward
forward();
OCR1A = DRIVE_MID;
}

if((PINC&0b00000001) == 0){   //color sensor
}

/*if(PIND&ob00000100) == 0){              //flame
stop();
PORTC = 0b00000000;
_delay_ms(3000);
PORTC = 0b00000010;
OCR1A = DRIVE_MIN;
_delay_ms(100);
forward();
_delay_ms(2000);
}
*/

}
}

void stop(){
OCR0 = 0;
}

void forward(){
OCR0 = 255;
CLEAR_BIT(PORTD, 3);
}

ISR(INT0_vect){
PORTC = 0b00000000;
_delay_ms(1000);
}


//ocr2

