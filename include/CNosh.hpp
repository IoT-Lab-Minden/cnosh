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
#include <NTPClient.h>
#include <RFID.hpp>
#include <RTClib.h>
#include <ServoEngine.hpp>
#include <WiFiUdp.h>

static Basecamp iot{Basecamp::SetupModeWifiEncryption::secured};
static RTC_DS3231 rtc;
static WiFiUDP ntpUDP;
static NTPClient timeClient(ntpUDP);

class CNosh {
  public:
    CNosh();
    ~CNosh() = default;
    void begin();

  private:
    LCD *lcd;
    RFID *rfid;
    Measure *measure;
    ServoEngine *servo;

    TaskHandle_t cnosh_handle;
    TaskHandle_t button_handle;
    TaskHandle_t lcd_handle;

    void init();
    void initConfiguration();
    void initWebserver(Configuration);
    void detectRFID();
    void checkFeeding(String);
    bool checkFeedingLock(String, String, int, int, int);
    bool checkFeedingExtra(String, String);
    void checkNextDay();
    void printLCD();
    void resetStatistics();
    void resetSystem();

    String getFormattedDateTime();

    static void startTaskButton(void *);
    static void startTaskCNosh(void *);
    static void startTaskLCD(void *);
};

#endif // CNOSH_H_