/*
   LCD - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#include <LCD.hpp>

LCD::LCD() {
    lcd = new LiquidCrystal_I2C(0x3F, lcdColumns, lcdRows); 
}

bool LCD::init() {

}

void LCD::printLine(String text, int row) {
    
}

void LCD::clear() {

}