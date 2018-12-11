/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#include <CNosh.hpp>

CNosh::CNosh() {
    settings = &jsonBuffer.createObject();
    lcd = new LCD();
    measure = new Measure();
}

bool CNosh::init() {
    iot.begin();
    // for using slave i2c-Bus
    // don't need this line when using master
    Wire.begin(SDA_SLAVE, SCL_SLAVE); 
    lcd->init();
    return true;
}

bool CNosh::begin() {
    lcd->printLine("PePe", 0);
    delay(2000);
    lcd->printLine("PePe", 1);
    delay(2000);
    return true;
}