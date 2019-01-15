/**
 * Firmware for ESP32 based cat food dispenser with Wi-Fi control
 * @file RFID.cpp
 * @author Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
 * @author Dario Leunig (dleunig@fh-bielefeld.de)
 * @brief Controls the MFRC522-RFID-Read/Write-Interface. See constant values
 * in Const.hpp for setup.
 * @version 0.1
 * @date 2019-01-05
 *
 * @copyright Copyright (c) 2019 \n Licensed under GPLv3. See LICENSE for
 * details.
 */
#include <RFID.hpp>

/**
 * @brief Construct a new RFID::RFID object.
 *
 */
RFID::RFID() { mfrc522 = new MFRC522(SS_PIN, RST_PIN); }

/**
 * @brief Initializes the MFRC522-object.
 *
 */
void RFID::init() {
    SPI.begin();
    mfrc522->PCD_Init();
    // mfrc522->PCD_DumpVersionToSerial();
}

/**
 * @brief Extracts the UID of the current unit detected.
 *
 * @return String The UID as string. (four byte: "0-255 0-255 0-255 0-255")
 */
String RFID::getUidAsString() {
    // byte nuidPICC[4];
    String nuidString = "";
    for (byte i = 0; i < mfrc522->uid.size; i++) {
        if (mfrc522->uid.uidByte[i] < 0x10) {
            nuidString.concat("0");
            nuidString.concat(mfrc522->uid.uidByte[i]);
            if (i != mfrc522->uid.size - 1) {
                nuidString.concat(" ");
            }
        } else {
            nuidString.concat(mfrc522->uid.uidByte[i]);
            if (i != mfrc522->uid.size - 1) {
                nuidString.concat(" ");
            }
        }
    }

    // Halt PICC
    mfrc522->PICC_HaltA();

    // Stop encryption on PCD
    mfrc522->PCD_StopCrypto1();

    return nuidString;
}

/**
 * @brief Looks for an rfid unit and reads it if one is present.
 *
 * @return true If a card was detected.
 * @return false Else.
 */
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
