/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#include <RFID.hpp>

RFID::RFID() {
    mfrc522 = new MFRC522 (SS_PIN, RST_PIN);
}

bool RFID::init() {
    while (!Serial); // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();
    mfrc522->PCD_Init(); // Init MFRC522
    mfrc522->PCD_DumpVersionToSerial();
    Serial.println("Scan PICC to see UID, SAK, type, and data blocks...");
}

void RFID::detectUnit() {
        // Look for new cards
        if ( ! mfrc522->PICC_IsNewCardPresent()) {
        return;
        }
        
        // Select one of the cards
        if ( ! mfrc522->PICC_ReadCardSerial()) {
        return;
        }
        
        // Dump debug info about the card; PICC_HaltA() is automatically called
        mfrc522->PICC_DumpToSerial(&(mfrc522->uid)); 
}