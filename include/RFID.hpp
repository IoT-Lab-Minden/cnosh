/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef RFID_h
#define RFID_h

#include <SPI.h>
#include <MFRC522.h>
#include <Const.hpp>

class RFID {
    public:
        RFID();
        ~RFID() = default;

        void dumpToSerial();
        bool init();
        int detectUnit();
        bool readUnitSerial();
    
    private:
       MFRC522 *rfid;

};

#endif