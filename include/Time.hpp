/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef Time_h
#define Time_h

#include <NTPClient.h>
#include <WiFiUdp.h>


class Time {
    public:
        Time();
        ~Time() = default;

        bool init();
        String getDate();
        String getTime();
    
    private:
       NTPClient *timeClient;
       WiFiUDP *ntpUDP;
       String formattedDate;
       String dayStamp;
       String timeStamp;
};

#endif