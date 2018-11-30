/*
   Measure - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef Measure_h
#define Measure_h

#include <Adafruit_VL53L0X.h>

class Measure {
    public:
        Measure();
        ~Measure() = default;

        bool init();
        double readDistance();
        
    private:
       Adafruit_VL53L0X *lox;
       VL53L0X_RangingMeasurementData_t *measure;

};

#endif