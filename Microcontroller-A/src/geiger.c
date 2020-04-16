#include "geiger.h"
#include "util.h"
#include <stddef.h>

uint32_t volatile _radiationCount = 0;
uint32_t volatile _noiseCount = 0;

struct GeigerCounter init_geiger(uint8_t signPin, uint8_t noisePin) {
    // init geiger struct
    struct GeigerCounter geiger;
    geiger.signPin = signPin;
    geiger.noisePin = noisePin;
    geiger._count = 0;
    geiger._historyIndex = 0;
    geiger._historyLength = 0;
    geiger._radiationCallback = NULL;
    geiger._noiseCallback = NULL;

    // setup geiger
    // enable 20kohm pull-up resistor for radiation and noise
    set_internal_pullup(signPin);
    set_internal_pullup(noisePin);

    // initialize _countHistory[]
    for (int i = 0; i < HISTORY_LENGTH; ++i) {
        geiger._countHistory[i] = 0;
    }

    geiger._previousTime = AVR_millis();
    geiger._previousHistoryTime = AVR_millis();

    return geiger;
}

void geiger_measure(struct GeigerCounter * geiger) {
    uint32_t currentTime = AVR_millis();

    if (currentTime - geiger->_previousTime >= PROCESS_PERIOD) {
        int currentCount = _radiationCount;
        int currentNoiseCount = _noiseCount;
        _radiationCount = 0;
        _noiseCount = 0;
        if (currentNoiseCount == 0) {
            // store count log
            geiger->_countHistory[geiger->_historyIndex] += currentCount;
            // add number of counts
            geiger->_count += currentCount;
        }

        // shift array for count log for each 6 seconds
        if (currentTime - geiger->_previousHistoryTime >= HISTORY_UNIT * 1000) {
            geiger->_previousHistoryTime += (uint32_t) (HISTORY_UNIT * 1000);
            geiger->_historyIndex = (geiger->_historyIndex + 1) % HISTORY_LENGTH;
            if (geiger->_historyLength < (HISTORY_LENGTH - 1)) {
                // Since, we overwrite the oldest value in the history,
                // the effective maximum length is HISTORY_LENGTH-1
                geiger->_historyLength++;
            }
            geiger->_count -= geiger->_countHistory[geiger->_historyIndex];
            geiger->_countHistory[geiger->_historyIndex] = 0;
        }
        // save time of current process period
        geiger->_previousTime = currentTime;
        // enable callbacks
        if (geiger->_noiseCallback && currentNoiseCount > 0) {
            geiger->_noiseCallback();
        }
        if (geiger->_radiationCallback && currentCount > 0) {
            geiger->_radiationCallback();
        }
    }
}

void registerRadiationCallback(struct GeigerCounter *geiger, void (*callback)(void)) {
    geiger->_radiationCallback = callback;
}

void registerNoiseCallback(struct GeigerCounter *geiger, void (*callback)(void)) {
    geiger->_noiseCallback = callback;
}

uint32_t integration_time(struct GeigerCounter *geiger) {
    return (geiger->_historyLength * HISTORY_UNIT * 1000UL + 
        geiger->_previousTime - geiger->_previousHistoryTime);
}

uint32_t currentRadiationCount() {
    return _radiationCount;
}

uint32_t radiationCount(struct GeigerCounter *geiger) {
    return geiger->_count;
}

double cpm(struct GeigerCounter *geiger) {
    // cpm = uSv x alpha
    double min = integration_time(geiger) / 60000.0;
    return (min > 0) ? radiationCount(geiger) / min : 0;
}

double uSvh(struct GeigerCounter *geiger) {
    return cpm(geiger) / kAlpha;
}

double uSvhError(struct GeigerCounter *geiger) {
    double min = integration_time(geiger) / 60000.0;
    return (min > 0) ? sqrt(radiationCount(geiger)) / min / kAlpha : 0;
}