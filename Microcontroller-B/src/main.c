#include <stdio.h>
#include <avr/io.h>
#include "benchmark.h"
#include "comm_spi.h"

// main for MCU B
int main(void) {
#ifdef CORE_DEBUG
    init_usart0();
#endif
    
    init_spi_slave();

    // TODO:
    // 1. finish communication (schema) with microcontroller A
    // 2. send benchmark result to uC A

    return 0;
}