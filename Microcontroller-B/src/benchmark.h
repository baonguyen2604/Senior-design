#include <inttypes.h>
#include <stdio.h>
#include <avr/io.h>

#ifndef TOTAL_DATA_SIZE
#define TOTAL_DATA_SIZE 666
#endif

/* align_mem :
	This macro is used to align an offset to point to a 32b value. It is used in the Matrix algorithm to initialize the input memory blocks.
*/
#define align_mem(x) (void *)(4 + (((uintptr_t)(x) - 1) & ~3))

int benchmark(void);

typedef int16_t MATDAT;
typedef int32_t MATRES;

typedef uint32_t CORETIMETYPE;
typedef uint32_t CORE_TICKS;

void start_time(void);
void stop_time(void);
CORE_TICKS get_time(void);
double time_in_secs(CORE_TICKS ticks);

/* Misc useful functions */
uint16_t crcu8(uint8_t data, uint16_t crc);
uint16_t crc16(int16_t newval, uint16_t crc);
uint16_t crcu16(uint16_t newval, uint16_t crc);
uint16_t crcu32(uint32_t newval, uint16_t crc);
uint8_t check_data_types();

typedef struct MAT_PARAM_S {
    int N;
    MATDAT *A;
    MATDAT *B;
    MATRES *C;
} mat_params;

typedef struct RESULTS_S {
    int16_t     seed1;
    int16_t     seed2;
    int16_t     seed3;
    void*       memblock[4];
    uint32_t    size;
    uint32_t    iterations;
    mat_params  mat;
    uint16_t    crc;
    uint16_t    crcmatrix;
    int16_t     err;
} results;

uint32_t init_matrix(uint32_t blksize, void *memblk, int32_t seed, mat_params *p);
uint16_t bench_matrix(mat_params *p, int16_t seed, uint16_t crc);