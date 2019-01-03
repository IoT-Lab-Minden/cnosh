/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)

   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef CNOSH_H_
#define CNOSH_H_

#include <Arduino.h>
#include <Basecamp.hpp>
#include <Const.hpp>
#include <LCD.hpp>
#include <Measure.hpp>
#include <RFID.hpp>
#include <ServoEngine.hpp>

static Basecamp iot{Basecamp::SetupModeWifiEncryption::secured};

class CNosh {
public:
  CNosh();
  ~CNosh() = default;

  bool begin();
  bool init();

private:
  LCD *lcd;
  RFID *rfid;
  Measure *measure;
  ServoEngine *servo;
  DynamicJsonBuffer jsonBuffer;
  JsonObject *settings;

  bool initConfiguration();
  void detectRFID();
  void checkFeeding();
  void printLCD();

  static void startTaskButton(void *);
  static void startTaskCNosh(void *);
  static void startTaskLCD(void *);
};

#endif // CNOSH_H_