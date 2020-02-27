#include <stdio.h>
#include <avr/io.h>
#include "benchmark/benchmark.h"

int main(void) {
#ifdef CORE_DEBUG
    init_usart0();
#endif
    

    return 1;
}