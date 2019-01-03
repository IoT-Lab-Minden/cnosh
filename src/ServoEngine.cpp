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
  servo->attach(SERVO_PIN);
}

void ServoEngine::rotate(int direction, int duration) {
  servo->write(direction);
  if (duration != 0) {
    delay(duration);
    this->stop();
  }
}

void ServoEngine::stop() { servo->write(95); }