
#define F_CPU 8000000UL
#include "AVR.h"
#include "nrf24_lib.h"
#include "nrf24_lib.c"
#include "ADC.h"
#include "UART.h"

short y = 0;
short x = 0;

uint8_t temp;
uint8_t data_array[2];
uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};

int main(void)
{
	DDRC = 0b11111111;
	DDRD = 0b00000010;
	
	UART_Init(9600);

	ADC_Init(ADC_REF_500V, ADC_8_BIT);
	sei();

	
	nrf24_init();
	nrf24_config(2,2);
	nrf24_tx_address(tx_address);
	nrf24_rx_address(rx_address);


	while(1)
	{

		x = ADC_Read8Bit(0);
		y = ADC_Read8Bit(1);

		data_array[0] = x;
		data_array[1] = y;
		
		UART_SEND_DECIMAL_INTEGER(x);
		UART_CRLF();

		nrf24_send(data_array);
		while(nrf24_isSending());
		
		nrf24_powerUpRx();
		_delay_ms(10);

	}
}