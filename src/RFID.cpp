/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#include <RFID.hpp>

RFID::RFID() {
    rfid = new MFRC522 (SS_PIN, RST_PIN);
}

bool RFID::init() {
    SPI.begin();
    rfid->PCD_Init(); // Init MFRC522
    rfid->PCD_DumpVersionToSerial();
    Serial.println("Scan PICC to see UID, SAK, type, and data blocks...");
}

void RFID::detectUnit() {
    if ( ! rfid->PICC_IsNewCardPresent()) {
        return;
    }
    // Select one of the cards
    if ( ! rfid->PICC_ReadCardSerial()) {
        return;
    }   
}

bool RFID::readUnitSerial() {
    // Dump debug info about the card; PICC_HaltA() is automatically called
    rfid->PICC_DumpToSerial(&(rfid->uid));
}