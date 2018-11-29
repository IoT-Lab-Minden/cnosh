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
}

bool CNosh::init() {
    lcd = new LCD();
}

bool CNosh::begin() {
    
}