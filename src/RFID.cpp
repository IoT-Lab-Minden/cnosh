/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#include <RFID.hpp>

RFID::RFID() {
    rfid = new MFRC522(SS_PIN, RST_PIN);
}

bool RFID::init() {

}

void RFID::dumpToSerial() {

}

int RFID::detectUnit() {

}

bool RFID::readUnitSerial() {

}