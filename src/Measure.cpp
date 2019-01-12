/**
 * @file Measure.cpp
 * @author Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
 * @author Dario Leunig (dleunig@fh-bielefeld.de)
 * @brief CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
 * @version 0.1
 * @date 2019-01-05
 *
 * @copyright Copyright (c) 2019
 * Licensed under GPLv3. See LICENSE for details.
 */
#include <Measure.hpp>

/**
 * @brief Construct a new Measure:: Measure object
 * 
 */
Measure::Measure() {
    lox = new Adafruit_VL53L0X();
    measure = new VL53L0X_RangingMeasurementData_t();
}

/**
 * @brief Initializes the Adafruit_VL53L0X-object
 *
 */
void Measure::init() {
    // wait until serial port opens for native USB devices
    while (!Serial) {
        delay(1);
    }

    Serial.println("Adafruit VL53L0X test");
    if (!lox->begin()) {
        Serial.println(F("Failed to boot VL53L0X"));
    }
}

/**
 * @brief Reads the output of the laser and returns
 * 
 * @return int distance in millimeter, -1 when laser is out of range
 */
int Measure::readDistance() {
    lox->rangingTest(measure,
                     false); // pass in 'true' to get debug data printout!
    if (measure->RangeStatus != 4) { // phase failures have incorrect data
        return measure->RangeMilliMeter;
    } else {
        return -1;
    }
}
