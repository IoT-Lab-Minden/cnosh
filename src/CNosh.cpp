/**
 * @file CNosh.cpp
 * @author Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
 * @author Dario Leunig (dleunig@fh-bielefeld.de)
 * @brief CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
 * @version 0.1
 * @date 2019-01-05
 * 
 * @copyright Copyright (c) 2019
 * Licensed under GPLv3. See LICENSE for details.
 */
#include <CNosh.hpp>

/**
 * @brief Construct a new CNosh::CNosh object
 * 
 */
CNosh::CNosh() {
  settings = &jsonBuffer.createObject();
  lcd = new LCD();
  measure = new Measure();
  servo = new ServoEngine();
  rfid = new RFID();
}

/**
 * @brief Setup for all components of cnosh
 * 
 * @return true if the initaltisation was succesfull
 * @return false if something doesn't work
 */
bool CNosh::init() {
  // the following line is for using slave i2c-Bus
  // don't need this line when using master
  Wire.begin(SDA_SLAVE, SCL_SLAVE);

  pinMode(BUTTON_PIN, INPUT_PULLDOWN);


  
  lcd->init();
  rfid->init();
  measure->init();

  initConfiguration();
  initWebserver(iot.configuration);
  iot.begin();
  return true;
}
/**
 * @brief For starting the Tasks und initialisation
 * 
 * @return true 
 * @return false 
 */
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
if (!iot.configuration.isKeySet(ConfigurationKey::cnoshConfiguration)) {
  iot.configuration.set(ConfigurationKey::time_1_h, "9");
  iot.configuration.set(ConfigurationKey::time_1_m, "15");
  iot.configuration.set(ConfigurationKey::time_2_h, "13");
  iot.configuration.set(ConfigurationKey::time_2_m, "0");
  iot.configuration.set(ConfigurationKey::time_3_h, "19");
  iot.configuration.set(ConfigurationKey::time_3_m, "30");
  iot.configuration.set(ConfigurationKey::time_4_h, "22");
  iot.configuration.set(ConfigurationKey::time_4_m, "00");
  iot.configuration.set(ConfigurationKey::time_amount_size, "M");

  iot.configuration.set(ConfigurationKey::c1_name, "Cat 1");
  iot.configuration.set(ConfigurationKey::c1_uid, "");
  iot.configuration.set(ConfigurationKey::c1_lastfeedingtime, "");
  iot.configuration.set(ConfigurationKey::c1_extra_amount_size, "");
  iot.configuration.set(ConfigurationKey::c1_extra_amount_number, "");
  iot.configuration.set(ConfigurationKey::c1_extra_amount_count, "");
  iot.configuration.set(ConfigurationKey::c1_extra_delay, "");
  iot.configuration.set(ConfigurationKey::c1_created, "0");
  iot.configuration.set(ConfigurationKey::c2_name, "Cat 2");
  iot.configuration.set(ConfigurationKey::c2_uid, "");
  iot.configuration.set(ConfigurationKey::c2_lastfeedingtime, "");
  iot.configuration.set(ConfigurationKey::c2_extra_amount_size, "");
  iot.configuration.set(ConfigurationKey::c2_extra_amount_number, "");
  iot.configuration.set(ConfigurationKey::c2_extra_amount_count, "");
  iot.configuration.set(ConfigurationKey::c2_extra_delay, "");
  iot.configuration.set(ConfigurationKey::c2_created, "0");
  iot.configuration.set(ConfigurationKey::c3_name, "Cat 3");
  iot.configuration.set(ConfigurationKey::c3_uid, "");
  iot.configuration.set(ConfigurationKey::c3_lastfeedingtime, "");
  iot.configuration.set(ConfigurationKey::c3_extra_amount_size, "");
  iot.configuration.set(ConfigurationKey::c3_extra_amount_number, "");
  iot.configuration.set(ConfigurationKey::c3_extra_amount_count, "");
  iot.configuration.set(ConfigurationKey::c3_extra_delay, "");
  iot.configuration.set(ConfigurationKey::c3_created, "0");

  iot.configuration.set(ConfigurationKey::startdate, "");
  iot.configuration.set(ConfigurationKey::last_savedate, "");
  iot.configuration.set(ConfigurationKey::last_feedingtime, "");
  iot.configuration.set(ConfigurationKey::total_amount_time, "");
  iot.configuration.set(ConfigurationKey::total_amount_extra, "");

  iot.configuration.set(ConfigurationKey::cnoshConfiguration, "true");
  iot.configuration.save();
}
iot.configuration.dump();

  // xTaskCreate(this->startTaskCNosh, "CNosh", 2048, this, 0, NULL);
  // xTaskCreate(this->startTaskButton, "Button", 2048, servo, 0, NULL);
  // xTaskCreate(this->startTaskLCD, "LCD", 2048, this, 0, NULL);
  return true;
}

void CNosh::initWebserver(Configuration config) {
  iot.web.server.on("/cnosh.json" , HTTP_GET, [&config, this](AsyncWebServerRequest * request)
	{
			AsyncJsonResponse *response = new AsyncJsonResponse();
			DynamicJsonBuffer _jsonBuffer;

			JsonObject &_jsonData = response->getRoot();
			JsonArray &cats = _jsonData.createNestedArray("cats");

      JsonObject &cat = cats.createNestedObject();
      cat["name"] = _jsonBuffer.strdup(iot.configuration.get("c1_name"));
      cat["uid"] = _jsonBuffer.strdup(iot.configuration.get("c1_uid"));
      cat["lastfeedingtime"] = _jsonBuffer.strdup(iot.configuration.get("c1_lastfeedingtime"));
      cat["extra_amount_size"] = _jsonBuffer.strdup(iot.configuration.get("c1_extra_amount_size"));
      cat["extra_amount_number"] = _jsonBuffer.strdup(iot.configuration.get("c1_extra_amount_number"));
      cat["extra_amount_count"] = _jsonBuffer.strdup(iot.configuration.get("c1_extra_amount_count"));
      cat["c1_extra_delay"] = _jsonBuffer.strdup(iot.configuration.get("c1_extra_delay"));
      cat["c1_created"] = _jsonBuffer.strdup(iot.configuration.get("c1_created"));

      JsonObject &feedingtimes = _jsonData.createNestedObject("feedingtimes");
      feedingtimes["time_1_h"] = _jsonBuffer.strdup(iot.configuration.get("time_1_h"));
      feedingtimes["time_1_m"] = _jsonBuffer.strdup(iot.configuration.get("time_1_m"));
      feedingtimes["time_2_h"] = _jsonBuffer.strdup(iot.configuration.get("time_2_h"));
      feedingtimes["time_2_m"] = _jsonBuffer.strdup(iot.configuration.get("time_2_m"));
      feedingtimes["time_3_h"] = _jsonBuffer.strdup(iot.configuration.get("time_3_h"));
      feedingtimes["time_3_m"] = _jsonBuffer.strdup(iot.configuration.get("time_3_m"));
      feedingtimes["time_4_h"] = _jsonBuffer.strdup(iot.configuration.get("time_4_h"));
      feedingtimes["time_4_m"] = _jsonBuffer.strdup(iot.configuration.get("time_4_m"));
      feedingtimes["time_amount_size"] = _jsonBuffer.strdup(iot.configuration.get("time_amount_size"));

      JsonObject &statistics = _jsonData.createNestedObject("statistics");
      statistics["configured"] = _jsonBuffer.strdup(iot.configuration.get("cnoshConfiguration"));
      statistics["startdate"] = _jsonBuffer.strdup(iot.configuration.get("startdate"));
      statistics["last_savedate"] = _jsonBuffer.strdup(iot.configuration.get("last_savedate"));
      statistics["last_feedingtime"] = _jsonBuffer.strdup(iot.configuration.get("last_feedingtime"));
      statistics["total_amount_time"] = _jsonBuffer.strdup(iot.configuration.get("total_amount_time"));
      statistics["total_amount_extra"] = _jsonBuffer.strdup(iot.configuration.get("total_amount_extra"));

			response->setLength();
			// NOTE: AsyncServer.send(ptr* foo) deletes `response` after async send.
			// As this is not documented in the header there: thanks for nothing.
			request->send(response);
	});
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
