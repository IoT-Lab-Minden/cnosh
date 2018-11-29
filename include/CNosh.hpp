/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef CNosh_h
#define CNosh_h

#include <Basecamp.hpp>
#include <LCD.hpp>
#include <RFID.hpp>
#include <Measure.hpp>
#include <ServoEngine.hpp>

class CNosh {
    public:
        CNosh();
        ~CNosh() = default;

        bool begin();
        bool init();
        
    private:
      LCD lcd;
      RFID rfid;
      Measure measure;
      ServoEngine servo;
      Basecamp iot; 

      JsonObject settings;

};

#endif