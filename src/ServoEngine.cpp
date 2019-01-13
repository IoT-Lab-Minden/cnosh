/**
 * @file ServoEngine.cpp
 * @author Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
 * @author Dario Leunig (dleunig@fh-bielefeld.de)
 * @brief CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
 * @version 0.1
 * @date 2019-01-05
 *
 * @copyright Copyright (c) 2019
 * Licensed under GPLv3. See LICENSE for details.
 */
#include <ServoEngine.hpp>

/**
 * @brief Construct a new Servo Engine:: Servo Engine object
 *
 */
ServoEngine::ServoEngine() {
    servo = new Servo();
    servo->attach(SERVO_PIN);
}

/**
 * @brief Rotats the
 *
 * @param direction see SERVO_ROTATE_FORWARD and SERVO_ROTATE_BACKWARD in
 * Const.hpp
 * @param duration time to activate the engine in milliseconds (3 seconds,
 * duration = 3000), if duration is 0 you have to call stop() to stop the
 * rotation
 */
void ServoEngine::rotate(int direction, int duration) {
    if (duration == 0) {
        // rotate everlasting
        servo->write(direction);
    } else {
        int duration_time;
        switch (duration) {
        case 1:
            duration_time = 1000;
            break;
        case 2:
            duration_time = 2000;
            break;
        case 3:
            duration_time = 3000;
            break;
        case 4:
            duration_time = 4000;
            break;
        }
        servo->write(direction);
        delay(duration_time);
        this->stop();
    }
}
/**
 * @brief Stops the engine with SERVO_STOP (see Const.hpp)
 *
 */
void ServoEngine::stop() { servo->write(SERVO_STOP); }