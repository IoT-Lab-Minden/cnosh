/**
 * @file LCD.cpp
 * @author Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
 * @author Dario Leunig (dleunig@fh-bielefeld.de)
 * @brief CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
 * @version 0.1
 * @date 2019-01-05
 *
 * @copyright Copyright (c) 2019
 * Licensed under GPLv3. See LICENSE for details.
 */
#include <LCD.hpp>
/**
 * @brief Construct a new LCD::LCD object
 *
 */
LCD::LCD() { lcd = new LiquidCrystal_I2C(LCD_ADRESS, LCD_COLUMNS, LCD_ROWS); }

/**
 * @brief Initzializes the LiquidCrystal_I2C-object and turns on the backlight
 * of the LCD display
 *
 */
void LCD::init() {
    // initialize LCD
    lcd->init();
    // turn on LCD backlight
    lcd->backlight();
}

/**
 * @brief Prints text to the specific row on the LCD diplay (2 rows and 16
 * columns)
 *
 * @param text 16 characters can shown at once, everything after this will be
 * cutted
 * @param row sets the row to print (0 or 1)
 */
void LCD::printLine(String text, int row) {
    if (row == 0 || row == 1) {
        lcd->setCursor(0, row);
        // print message
        lcd->print(text);
    }
}

/**
 * @brief Clears the complete display
 *
 */
void LCD::clear() { lcd->clear(); }