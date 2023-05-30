#include <CEC_UART.h>
#include <CEC_ADC.h>
#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

short flame = 0;

int main(void){
DDRB = 0b00000001;
UART_Init(9600);
ADC_Init(ADC_REF_500V, ADC_8_BIT);
sei();


while(1){
PORTB = 0b00000000;
flame = ADC_Read8Bit(3);
UART_SEND_DECIMAL_INTEGER(flame);
UART_CRLF();


}
}




