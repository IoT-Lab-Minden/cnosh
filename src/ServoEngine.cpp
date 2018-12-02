/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#include <ServoEngine.hpp>

ServoEngine::ServoEngine() {
    servo = new Servo();
}

void ServoEngine::rotate(int direction, int duration) {

}

void ServoEngine::stop() {

}