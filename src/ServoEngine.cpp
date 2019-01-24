/**
 * Firmware for ESP32 based cat food dispenser with Wi-Fi control
 * @file ServoEngine.cpp
 * @author Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
 * @author Dario Leunig (dleunig@fh-bielefeld.de)
 * @brief Controls the Servo-Engine. See constant values in Const.hpp for setup.
 * @version 0.1
 * @date 2019-01-05
 *
 * @copyright Copyright (c) 2019 \n Licensed under GPLv3. See LICENSE for
 * details.
 */
#include <ServoEngine.hpp>

/**
 * @brief Construct a new Servo Engine:: Servo Engine object.
 *
 */
ServoEngine::ServoEngine() {
    servo = new Servo();
    servo->attach(SERVO_PIN);
}

/**
 * @brief Rotats the servo in the specific direction. See constant values in
 * Const.hpp for setup.
 *
 * @param direction See SERVO_ROTATE_FORWARD and SERVO_ROTATE_BACKWARD in
 * Const.hpp
 * @param duration Time to activate the engine in milliseconds (3 seconds,
 * duration = 3000), if duration is 0 you have to call stop() to stop the
 * rotation.
 */
void ServoEngine::rotate(int direction, int duration) {
    servo->write(direction);
}
/**
 * @brief Stops the engine with SERVO_STOP (see Const.hpp).
 *
 */
void ServoEngine::stop() { servo->write(SERVO_STOP); }