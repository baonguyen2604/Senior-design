#define SPI_DDR     DDRB
#define CS          PINB2
#define MOSI        PINB3
#define MISO        PINB4
#define SCK         PINB5

void init_spi_slave(void);
unsigned char spi_transceiver(void);
