#include <stdio.h>
#include <avr/pgmspace.h>
#include "uart.h"

static FILE usart0_str = FDEV_SETUP_STREAM(send_byte_usart0, NULL, _FDEV_SETUP_WRITE);

int send_byte_usart0(char u8_data, FILE *stream) {
    if (u8_data == '\n') {
        send_byte_usart0('\r', 0);
    }

    // wait while previous byte is completed
	while (!(UCSR0A&(1<<UDRE0))) {

    }
	// Transmit data
	UDR0 = u8_data;
	return 0;
}

void init_usart0(void) {
    // Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//enable transmission and reception
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);

	stdout = &usart0_str;
}