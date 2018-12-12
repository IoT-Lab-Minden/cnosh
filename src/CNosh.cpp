/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#include <CNosh.hpp>

CNosh::CNosh() {
    settings = &jsonBuffer.createObject();
    lcd = new LCD();
    measure = new Measure();
}

bool CNosh::init() {
    iot.begin();
    this->initConfiguration();
    // for using slave i2c-Bus
    // don't need this line when using master
    Wire.begin(SDA_SLAVE, SCL_SLAVE); 
    lcd->init();
    return true;
}

bool CNosh::begin() {
    lcd->printLine("PePe", 0);
    delay(2000);
    iot.configuration.dump();
    lcd->printLine("PePe", 1);
    delay(2000);
    return true;
}

bool CNosh::initConfiguration() {
    if(!iot.configuration.isKeySet(ConfigurationKey::cnoshConfiguration)) {
        iot.configuration.set(ConfigurationKey::time_1_h, "9");
        iot.configuration.set(ConfigurationKey::time_1_m, "15");
        iot.configuration.set(ConfigurationKey::time_2_h, "13");
        iot.configuration.set(ConfigurationKey::time_2_m, "0");
        iot.configuration.set(ConfigurationKey::time_3_h, "19");
        iot.configuration.set(ConfigurationKey::time_3_m, "30");
        iot.configuration.set(ConfigurationKey::time_4_h, "22");
        iot.configuration.set(ConfigurationKey::time_4_m, "00");
        iot.configuration.set(ConfigurationKey::time_amount_size, "M");
        iot.configuration.set(ConfigurationKey::cnoshConfiguration, "true");
        iot.configuration.save();
    }
    return true;
}