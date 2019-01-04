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
  servo = new ServoEngine();
  rfid = new RFID();
}

bool CNosh::init() {
  // the following line is for using slave i2c-Bus
  // don't need this line when using master
  Wire.begin(SDA_SLAVE, SCL_SLAVE);

  pinMode(BUTTON_PIN, INPUT_PULLDOWN);

  iot.begin();
  lcd->init();
  rfid->init();
  measure->init();

  this->initConfiguration();
  return true;
}

bool CNosh::begin() {
  Serial.println("Hello World!");
  delay(3000);
  return true;
}

void CNosh::startTaskCNosh(void *cnoshObj) {
  CNosh *cn = (CNosh *)cnoshObj;
  while (1) {
    cn->detectRFID();
    // cn->checkFeeding();
  }
}

void CNosh::startTaskLCD(void *cnoshObj) {
  CNosh *cn = (CNosh *)cnoshObj;
  while (1) {
    cn->printLCD();
    vTaskDelay(2000);
  }
}

void CNosh::startTaskButton(void *servoObj) {
  int press1 = 0;
  ServoEngine *servoTest = (ServoEngine *)servoObj;
  while (1) {
    press1 = digitalRead(BUTTON_PIN);
    if (press1 == LOW) {
      servoTest->stop();
    } else {
      servoTest->rotate(180, 0);
    }
  }
}

bool CNosh::initConfiguration() {
  /*  if (!iot.configuration.isKeySet(ConfigurationKey::cnoshConfiguration))
{
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
iot.configuration.dump(); */

  xTaskCreate(this->startTaskCNosh, "CNosh", 2048, this, 0, NULL);
  xTaskCreate(this->startTaskButton, "Button", 2048, servo, 0, NULL);
  xTaskCreate(this->startTaskLCD, "LCD", 2048, this, 0, NULL);
  return true;
}

void CNosh::detectRFID() { rfid->detectUnit(); }

void CNosh::checkFeeding() {
  // check feeding time
}

void CNosh::printLCD() {
  String level = "FillLevel: ";
  int distance = measure->readDistance();
  if (distance < 0) {
    level.concat("outofrange");
  } else {
    level.concat(distance);
  }
  lcd->clear();
  lcd->printLine("Welcome to CNosh", 0);
  lcd->printLine(level, 1);
}
