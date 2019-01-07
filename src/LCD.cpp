/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)

   Licensed under GPLv3. See LICENSE for details.
   */

#include <LCD.hpp>

LCD::LCD() { lcd = new LiquidCrystal_I2C(LCD_ADRESS, LCD_COLUMNS, LCD_ROWS); }

bool LCD::init() {
    // initialize LCD
    lcd->init();
    // turn on LCD backlight
    lcd->backlight();
    return true;
}

void LCD::printLine(String text, int row) {
    lcd->setCursor(0, row);
    // print message
    lcd->print(text);
}

void LCD::clear() { lcd->clear(); }