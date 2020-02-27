#include "benchmark.h"

static uint16_t matrix_known_crc = (uint16_t) 0xa2e1;

void *iterate(void *pres) {
    uint32_t i;
    uint16_t crc;
    results *res = (results *)pres;
    uint32_t iterations = res->iterations;
    res->crc = 13198;
    res->crcmatrix = 0;

    for (i = 0; i < iterations; ++i) {
        crc = bench_matrix(&(res->mat), 0xcc, res->crc);
        res->crc = crcu16(crc, res->crc);

        crc = bench_matrix(&(res->mat), 0x44, res->crc);
        res->crc = crcu16(crc, res->crc);

        crc = bench_matrix(&(res->mat), 0x44, res->crc);
        res->crc = crcu16(crc, res->crc);

        crc = bench_matrix(&(res->mat), 0xcc, res->crc);
        res->crc = crcu16(crc, res->crc);
        if (i == 0)
            res->crcmatrix = res->crc;
    }
    return NULL;
}

int benchmark(void) {
    uint16_t i, j = 0;
    int16_t known_id = -1, total_errors = 0;
    uint16_t seedcrc = 0;
    results result;
    CORE_TICKS total_time;

    uint8_t stack_memblock[TOTAL_DATA_SIZE];

    result.seed1 = 0x3415;
    result.seed2 = 0x3415;
    result.seed3 = 0x66;
    result.iterations = 1;
    result.size = TOTAL_DATA_SIZE;

    result.memblock[0] = stack_memblock;
    result.err = 0;
    result.memblock[1] = result.memblock[0];
    mat_params mat;

    init_matrix(result.size, result.memblock[1], (int32_t) result.seed1 | ((int32_t) result.seed2 << 16), &result.mat);

    init_timer();
    start_time();

    iterate(&result);

    stop_time();
    total_time = get_time();

    seedcrc = crc16(result.seed1,seedcrc);
	seedcrc = crc16(result.seed2,seedcrc);
	seedcrc = crc16(result.seed3,seedcrc);
	seedcrc = crc16(result.size,seedcrc);

#ifdef CORE_DEBUG
    if (result.crcmatrix != matrix_known_crc) {
        printf("ERROR! matrix crc 0x%04x - should be 0x%04x\n", result.crcmatrix, matrix_known_crc);
    }

    printf("Benchmark data size: %lu\n", (long unsigned) result.size);
    printf("Total ticks: %lu\n", (long unsigned) total_time);

    printf("Total time (secs): %f\n", time_in_secs(total_time));
    printf("seedcrc: 0x%04x\n",seedcrc);
    printf("[%d]crcmatrix: 0x%04x\n",i,result.crcmatrix);
#endif

    return total_errors;
} 