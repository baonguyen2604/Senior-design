#include <avr/io.h>

#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void init_usart0(void);
int send_byte_usart0(char u8_data, FILE *stream);