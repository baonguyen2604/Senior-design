#include "comm_spi.h"

void init_spi_master(void) {
    // MOSI, SCK as output
    SPI_DDR |= (1 << MOSI) | (1 << SCK);

    // enable SPI, set as slave, clock to fpcu/128;
    SPCR |= (1 << SPE) | (1 << SPR1) | (1 << SPR0);
}

uint8_t spi_transceiver(uint8_t data) {
    // load data into buffer
    SPDR = data;

    // wait until transmission completes
    while (!(SPSR & (1 << SPIF))) {

    }

    return SPSR;
}