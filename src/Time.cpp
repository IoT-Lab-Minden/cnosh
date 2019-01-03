/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)

   Licensed under GPLv3. See LICENSE for details.
   */

#include <Time.hpp>

Time::Time() {
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP);
}

bool Time::init() {}

String Time::getDate() {}

String Time::getTime() {}