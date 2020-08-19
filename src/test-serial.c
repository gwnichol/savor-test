#define F_CPU 16000000
#ifndef BAUD
    #define BAUD 38400
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <savor/spi.h>
#include <savor/serial.h>

int main(){
	char count = 0;

	DDRC |= (1 << 5);

    uint16_t ubrr = serial_ubrr(F_CPU, BAUD);

    serial_init(ubrr);

	while(1){
        printf("Ubrr: %d, Count: %d\n", ubrr, count);
		count++;
		_delay_ms(1000);
	}
}
