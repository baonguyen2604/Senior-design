#include "sensors.h"
#include "geiger.h"

// TODO: finish sensors initialization and reading for IR receiver and photoresistor

struct SensorMeasures {
    double uSvh;
    uint32_t ir_value;
    uint32_t photo_r_value;
};

uint8_t initialize_sensors() {
    struct GeigerCounter geiger = init_geiger(2, 3);
    // init_photoresistor();
    // init_IRsensor();

    return 0;
}

struct SensorMeasures read_sensors(struct GeigerCounter *geiger) {
    struct SensorMeasures measures;

    geiger_measure(geiger);

    measures.uSvh = uSvh(geiger);
    // measures.ir_value = ir_measure();
    // measures.photo_r_value = photo_measures();

    return measures;
}

