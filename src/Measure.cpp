/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)

   Licensed under GPLv3. See LICENSE for details.
   */

#include <Measure.hpp>

Measure::Measure() {
    lox = new Adafruit_VL53L0X();
    measure = new VL53L0X_RangingMeasurementData_t();
}

bool Measure::init() {
    // wait until serial port opens for native USB devices
    while (!Serial) {
        delay(1);
    }

    Serial.println("Adafruit VL53L0X test");
    if (!lox->begin()) {
        Serial.println(F("Failed to boot VL53L0X"));
    }
}

int Measure::readDistance() {
    lox->rangingTest(measure,
                     false); // pass in 'true' to get debug data printout!
    if (measure->RangeStatus != 4) { // phase failures have incorrect data
        return measure->RangeMilliMeter;
    } else {
        return -1;
    }
}
