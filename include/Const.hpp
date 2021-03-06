/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)

   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef CNOSH_CONST_H
#define CNOSH_CONST_H

// LCD
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;
const int LCD_ADRESS = 39; // 0x3F //0x27
const int SDA_SLAVE = 25;
const int SCL_SLAVE = 26;
// RFID
const int RST_PIN = 2;
const int SS_PIN = 5;
// Servo
const int SERVO_PIN = 12;
const int SERVO_ROTATE_FORWARD = 180;
const int SERVO_ROTATE_BACKWARD = 60;
const int SERVO_STOP = 95;
// Button
const int BUTTON_PIN = 13;
// Clock
const int TIMEZONE_OFFSET = 3600;
// Other
const int CNOSH_CAT_NUM = 3;


#endif // CNOSH_CONST_h