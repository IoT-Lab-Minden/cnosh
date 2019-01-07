/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)

   Licensed under GPLv3. See LICENSE for details.
   */

#include <RFID.hpp>

RFID::RFID() { mfrc522 = new MFRC522(SS_PIN, RST_PIN); }

bool RFID::init() {
    SPI.begin();
    mfrc522->PCD_Init();
    // mfrc522->PCD_DumpVersionToSerial();
}

String RFID::getUID() {
    byte nuidPICC[4];

    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522->PICC_GetType(mfrc522->uid.sak);
    Serial.println(mfrc522->PICC_GetTypeName(piccType));

    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        return "your TAg ";
    }

    if (mfrc522->uid.uidByte[0] != nuidPICC[0] ||
        mfrc522->uid.uidByte[1] != nuidPICC[1] ||
        mfrc522->uid.uidByte[2] != nuidPICC[2] ||
        mfrc522->uid.uidByte[3] != nuidPICC[3]) {
        Serial.println(F("A new card has been detected."));

        // Store NUID into nuidPICC array
        for (byte i = 0; i < 4; i++) {
            nuidPICC[i] = mfrc522->uid.uidByte[i];
        }

        Serial.println(F("The NUID tag is:"));
        Serial.print(F("In hex: "));
        printHex(mfrc522->uid.uidByte, mfrc522->uid.size);
        Serial.println();
        Serial.print(F("In dec: "));
        printDec(mfrc522->uid.uidByte, mfrc522->uid.size);
        Serial.println();
    } else
        Serial.println(F("Card read previously."));

    // Halt PICC
    mfrc522->PICC_HaltA();

    // Stop encryption on PCD
    mfrc522->PCD_StopCrypto1();
}

bool RFID::detectUnit() {
    // Look for new cards
    if (!mfrc522->PICC_IsNewCardPresent()) {
        return false;
    }

    // Select one of the cards
    if (!mfrc522->PICC_ReadCardSerial()) {
        return false;
    }

    return true;
}

void RFID::printHex(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
void RFID::printDec(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
    }
}
