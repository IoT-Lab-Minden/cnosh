/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef LCD_h
#define LCD_h

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class LCD {
    public:
        LCD();
        ~LCD() = default;

        void printLine(String, int);
        bool init();
        void clear();
    
    private:
       LiquidCrystal_I2C lcd; 

};

#endif