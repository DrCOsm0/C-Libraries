#define F_CPU 8000000UL

#include <nrf24.h>
#include <nrf24.c>

uint8_t data_array[2];

uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};

int main(){
	
	while(1){



		if(nrf24_dataReady()){
		
			nrf24_getData(data_array);
			x = data_array[0];
			y = data_array[1];		        
		}
	}
}









