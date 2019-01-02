/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef CNOSH_RFID_H
#define CNOSH_RFID_H

#include <SPI.h>
#include <MFRC522.h>
#include <Const.hpp>

class RFID {
    public:
        RFID();
        ~RFID() = default;

        void dumpToSerial();
        bool init();
        void detectUnit();
    
    private:
       MFRC522 *mfrc522;

};

#endif // CNOSH_RFID_H