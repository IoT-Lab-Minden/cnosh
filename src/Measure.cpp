/*
   Measure - Firmware for ESP32 based cat food dispenser with Wi-Fi control
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

}

double Measure::readDistance() {

}
