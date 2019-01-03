/*
   Measure - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)

   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef CNOSH_MEASURE_H
#define CNOSH_MEASURE_H

#include <Adafruit_VL53L0X.h>

class Measure {
public:
  Measure();
  ~Measure() = default;

  bool init();
  int readDistance();

private:
  Adafruit_VL53L0X *lox;
  VL53L0X_RangingMeasurementData_t *measure;
};

#endif // CNOSH_MEASURE_H