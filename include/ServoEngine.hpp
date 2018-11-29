/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef ServoEngine_h
#define ServoEngine_h

#include <Servo.h>

class ServoEngine {
    public:
        ServoEngine();
        ~ServoEngine() = default;

        void rotate();
        void stop();
    
    private:
       Servo servo; 

};

#endif