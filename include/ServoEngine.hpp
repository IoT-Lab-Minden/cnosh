/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#ifndef CNOSH_SERVOENGINE_H
#define CNOSH_SERVOENGINE_H

#include <Servo.h>
#include <Const.hpp>

class ServoEngine {
    public:
        ServoEngine();
        ~ServoEngine() = default;

        void rotate(int, int);
        void stop();
    
    private:
       Servo *servo; 

};

#endif // CNOSH_SERVOENGINE_H