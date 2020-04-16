#include <stdio.h>
#include <avr/io.h>
#include "comm_spi.h"


// main for MCU A
int main(void) {
#ifdef CORE_DEBUG
    init_usart0();
#endif
    
    init_spi_master();

    // transmit 1 to start communication with slave
    // expect 0 back
    uint8_t data = spi_transceiver(1);
    
    if (data != 0) {
        printf("ERROR: failed to initialize SPI connection with uC B\n");
    }

    // initialize sensors
    uint8_t ret = initialize_sensors();

    if (ret != 0) {
        printf("ERROR: failed to initialize sensors\n");
    }

    // TODO: 
    // 1. finish communication protocol (schema) with microcontroller B
    // 2. report sensors reading
    // 3. report benchmarck reading

    return 0;
}