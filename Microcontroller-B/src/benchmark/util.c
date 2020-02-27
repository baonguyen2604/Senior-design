#include "benchmark.h"

CORETIMETYPE barebones_clock() {
	return (uint32_t) TCNT1;
}

#define CLOCKS_PER_SEC 15625.0
#define TIMER_RES_DIVIDER 1

#define GETMYTIME(_t) (*_t = barebones_clock())
#define TIMEDIFF(fin, ini) ((fin) - (ini))
#define TICKS_PER_SEC (CLOCKS_PER_SEC / TIMER_RES_DIVIDER)

static CORETIMETYPE start_time_val, stop_time_val;

void init_timer() {
	TCNT1 = 0;
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void start_time(void) {
	init_timer();
	GETMYTIME(&start_time_val);
}

void stop_time(void) {
	GETMYTIME(&stop_time_val);
}

CORE_TICKS get_time(void) {
	CORE_TICKS elapsed = (CORE_TICKS)(TIMEDIFF(stop_time_val, start_time_val));
	return elapsed;
}

double time_in_secs(CORE_TICKS ticks) {
	double retval = ((double) ticks / (double) TICKS_PER_SEC);
	return retval;
}

uint16_t crcu8(uint8_t data, uint16_t crc) {
    uint8_t i = 0, x16 = 0, carry = 0;

    for (i = 0; i < 8; i++) {
		x16 = (uint8_t)((data & 1) ^ ((uint8_t)crc & 1));
		data >>= 1;

		if (x16 == 1)
		{
		   crc ^= 0x4002;
		   carry = 1;
		}
		else 
			carry = 0;
		crc >>= 1;
		if (carry)
		   crc |= 0x8000;
		else
		   crc &= 0x7fff;
    }
	return crc;
}

uint16_t crcu16(uint16_t newval, uint16_t crc) {
	crc=crcu8((uint8_t) (newval), crc);
	crc=crcu8((uint8_t) ((newval)>>8), crc);
	return crc;
}
uint16_t crcu32(uint32_t newval, uint16_t crc) {
	crc=crc16((int16_t) newval, crc);
	crc=crc16((int16_t) (newval>>16), crc);
	return crc;
}
uint16_t crc16(int16_t newval, uint16_t crc) {
	return crcu16((uint16_t) newval, crc);
}

uint8_t check_data_types() {
	uint8_t retval=0;
	if (sizeof(uint8_t) != 1) {
		printf("ERROR: ee_u8 is not an 8b datatype!\n");
		retval++;
	}
	if (sizeof(uint16_t) != 2) {
		printf("ERROR: ee_u16 is not a 16b datatype!\n");
		retval++;
	}
	if (sizeof(int16_t) != 2) {
		printf("ERROR: ee_s16 is not a 16b datatype!\n");
		retval++;
	}
	if (sizeof(int32_t) != 4) {
		printf("ERROR: ee_s32 is not a 32b datatype!\n");
		retval++;
	}
	if (sizeof(uint32_t) != 4) {
		printf("ERROR: ee_u32 is not a 32b datatype!\n");
		retval++;
	}
	if (sizeof(uintptr_t) != sizeof(int *)) {
		printf("ERROR: ee_ptr_int is not a datatype that holds an int pointer!\n");
		retval++;
	}
	if (retval>0) {
		printf("ERROR: Please modify the datatypes in core_portme.h!\n");
	}
	return retval;
}