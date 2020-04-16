#include <stdint.h>
#ifndef HISTORY_LENGTH
#define HISTORY_LENGTH 200
#endif

#define HISTORY_UNIT 6
#define PROCESS_PERIOD 160

static const double kAlpha = 53.032;

struct GeigerCounter {
    // public
    uint8_t signPin;
    uint8_t noisePin;

    // private

    // history of count rates
    uint32_t _countHistory[HISTORY_LENGTH];
    uint32_t _previousTime;
    uint32_t _previousHistoryTime;

    // current count (sum of count in _countHistory)
    uint32_t _count;
    // position of current count rate on _countHistory[]
    uint8_t _historyIndex;
    // current length of count history
    uint8_t _historyLength;
    // pin settings
    uint8_t _signPin;
    uint8_t _noisePin;
    // user callbacks
    void (*_radiationCallback)(void);
    void (*_noiseCallback)(void);
};

struct GeigerCounter init_geiger(uint8_t signPin, uint8_t noisePin);

void geiger_measure(struct GeigerCounter * geiger);

uint32_t integration_time(struct GeigerCounter *geiger);

uint32_t currentRadiationCount();

uint32_t radiationCount(struct GeigerCounter *geiger);

double cpm(struct GeigerCounter *geiger);

double uSvh(struct GeigerCounter *geiger);

double uSvhError(struct GeigerCounter *geiger);

void registerRadiationCallback(struct GeigerCounter *geiger, void (*callback)(void));

void registerNoiseCallback(struct GeigerCounter *geiger, void (*callback)(void));