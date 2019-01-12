/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)

   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef CNOSH_RFID_H
#define CNOSH_RFID_H

#include <Const.hpp>
#include <MFRC522.h>
#include <SPI.h>

class RFID {
  public:
    RFID();
    ~RFID() = default;

    void init();
    bool detectUnit();
    String getUidAsString();

  private:
    MFRC522 *mfrc522;
};

#endif // CNOSH_RFID_H