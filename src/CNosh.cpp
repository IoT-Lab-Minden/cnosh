/**
 * Firmware for ESP32 based cat food dispenser with Wi-Fi control
 * @file CNosh.cpp
 * @author Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
 * @author Dario Leunig (dleunig@fh-bielefeld.de)
 * @brief Main class of CNosh food dispenser firmware
 * @version 0.1
 * @date 2019-01-05
 *
 * @copyright Copyright (c) 2019 \n Licensed under GPLv3. See LICENSE for
 * details.
 */
#include <CNosh.hpp>

/**
 * @brief Construct a new CNosh::CNosh object with all needed components.
 *
 */
CNosh::CNosh() {

    lcd = new LCD();
    measure = new Measure();
    servo = new ServoEngine();
    rfid = new RFID();
}

/**
 * @brief Starts the Firmware. Needs to be executed just once.
 */
void CNosh::begin() { init(); }

/**
 * @brief Initialize all components of cnosh, including the webserver and cnosh
 * configuration data.
 */
void CNosh::init() {
    iot.begin();
    initWebserver(iot.configuration);

    // // the following line is for using slave i2c-Bus
    // // don't need this line when using master
    Wire.begin(SDA_SLAVE, SCL_SLAVE);

    pinMode(BUTTON_PIN, INPUT_PULLDOWN);

    // initialize the RT clock and NTP clock
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
    }
    if (!iot.configuration.get(ConfigurationKey::cnoshConfiguration)
             .equalsIgnoreCase("1")) {
        rtc.adjust(DateTime(__DATE__, __TIME__));
    }

    // timeClient.begin();
    // timeClient.setTimeOffset(TIMEZONE_OFFSET);

    // initialize all components
    lcd->init();
    rfid->init();
    measure->init();

    initConfiguration();
}

/**
 * @brief FreeRTOS like taskfunction to start the checking process of rfid and
 * feedingtimes (infinity loop).
 *
 * @param cnoshObj Takes the complete cnosh object it self, to have access to
 * all components.
 */
void CNosh::startTaskCNosh(void *cnoshObj) {
    CNosh *cn = (CNosh *)cnoshObj;
    while (1) {
        // cn->detectRFID();
        cn->checkFeeding("");

        if (cn->rfid->detectUnit()) {
            unsigned long start = millis();
            unsigned long duration = 2000;

            Serial.println(cn->rfid->getUidAsString());
            DateTime dt = rtc.now();
            Serial.print(dt.hour());
            Serial.print(":");
            Serial.println(dt.minute());

            String test = "Range:";
            test.concat(cn->measure->readDistance());
            Serial.println(test);
            
            cn->lcd->clear();
            cn->lcd->printLine("Hallo", 0);
            cn->lcd->printLine("RFID!", 1);

                        while (millis() <= start + duration) {
                cn->servo->rotate(SERVO_ROTATE_FORWARD, 0);
            }
        }
        cn->servo->stop();
    }
}

/**
 * @brief FreeRTOS like taskfunction to start the output on the LCD-display
 * (infinity loop). It checks the WLAN configuration for it self and prints the
 * right output to the LCD-diplay.
 *
 * @param cnoshObj Takes the complete cnosh object, to have access to all
 * components.
 */
void CNosh::startTaskLCD(void *cnoshObj) {
    CNosh *cn = (CNosh *)cnoshObj;
    while (1) {
        cn->printLCD();
    }
}

/**
 * @brief FreeRTOS like taskfunction to start the checking process of the
 * dispending button (infinity loop).
 *
 * @param servoObj Takes the Servo-object to control the engine.
 */
void CNosh::startTaskButton(void *servoObj) {
    int press1 = 0;
    ServoEngine *servoTest = (ServoEngine *)servoObj;
    while (1) {
        press1 = digitalRead(BUTTON_PIN);
        if (press1 == HIGH) {
            servoTest->rotate(SERVO_ROTATE_FORWARD, 0);
        }
    }
}

/**
 * @brief This method detects a rfid-unit and checks if it is warranted to get
 * fooder.
 *
 */
void CNosh::detectRFID() {
    if (rfid->detectUnit()) {

        if (iot.configuration.get("c1_uid").equals(rfid->getUidAsString())) {
            Serial.print(iot.configuration.get("c1_name"));
            Serial.println(" wurde erkannt!");
            checkFeeding("c1");
        }
        if (iot.configuration.get("c2_uid").equals(rfid->getUidAsString())) {
            Serial.print(iot.configuration.get("c2_name"));
            checkFeeding("c2");
            Serial.println(" wurde erkannt!");
        }
        if (iot.configuration.get("c3_uid").equals(rfid->getUidAsString())) {
            Serial.print(iot.configuration.get("c3_name"));
            checkFeeding("c3");
            Serial.println(" wurde erkannt!");
        }
    }
}

/**
 * @brief Resets the system to default including cnosh and WLAN configuration
 * data. Reboot after done.
 *
 */
void CNosh::resetSystem() {}

/**
 * @brief Resets the statistics of cnosh and all configured cats.
 *
 */
void CNosh::resetStatistics() {}

/**
 * @brief Checks the configured feeding times and if a cat is warranted to get
 * extra fooder. If there is a match it will dispend fooder and saves the last
 * feeding time to the memory.
 *
 * @param cat If input is a cat ("c1","c2","c3") it checks extra fooder, else if
 * its an empty string("") it checks the regular feeding times.
 */
void CNosh::checkFeeding(String cat) {
    DateTime now = rtc.now();
    bool time_output = false;
    bool cat_output = false;
    if (cat.equalsIgnoreCase("")) {
        // function call without cat_uid
                if (iot.configuration.get("time_1_h").toInt() ==
                                  now.hour() &&
                              iot.configuration.get("time_1_m").toInt() ==
                                  now.minute()) {
            servo->rotate(SERVO_ROTATE_FORWARD,
                          iot.configuration.get("time_amount_size").toInt());
            time_output = true;
        }
        if (iot.configuration.get("time_2_h").toInt() == now.hour() &&
            iot.configuration.get("time_2_m").toInt() == now.minute()) {
            servo->rotate(SERVO_ROTATE_FORWARD,
                          iot.configuration.get("time_amount_size").toInt());
            time_output = true;
        }
        if (iot.configuration.get("time_3_h").toInt() == now.hour() &&
            iot.configuration.get("time_3_m").toInt() == now.minute()) {
            servo->rotate(SERVO_ROTATE_FORWARD,
                          iot.configuration.get("time_amount_size").toInt());
            time_output = true;
        }
        if (iot.configuration.get("time_4_h").toInt() == now.hour() &&
            iot.configuration.get("time_4_m").toInt() == now.minute()) {
            servo->rotate(SERVO_ROTATE_FORWARD,
                          iot.configuration.get("time_amount_size").toInt());
            time_output = true;
        }
    } else {
        if (cat.equals("c1")) {
            if (checkFeedingLock("c1_lastfeedingtime", "c1_extra_delay",
                                 now.hour(), now.minute(), now.day()) &&
                checkFeedingExtra("c1_extra_amount_number",
                                  "c1_extra_amount_count")) {
                servo->rotate(
                    SERVO_ROTATE_FORWARD,
                    iot.configuration.get("c1_extra_amount_size").toInt());
                iot.configuration.set("c1_last_feedingtime",
                                      getFormattedDateTime());

                int extra_count =
                    iot.configuration.get("c1_extra_amount_count").toInt();
                String parse_count = "";
                extra_count++;
                parse_count.concat(extra_count);
                iot.configuration.set("c1_extra_amount_count", parse_count);
                cat_output = true;
            }
        }
        if (cat.equals("c2")) {
            if (checkFeedingLock("c2_lastfeedingtime", "c2_extra_delay",
                                 now.hour(), now.minute(), now.day()) &&
                checkFeedingExtra("c2_extra_amount_number",
                                  "c2_extra_amount_count")) {
                servo->rotate(
                    SERVO_ROTATE_FORWARD,
                    iot.configuration.get("c2_extra_amount_size").toInt());
                iot.configuration.set("c2_last_feedingtime",
                                      getFormattedDateTime());
                int extra_count =
                    iot.configuration.get("c2_extra_amount_count").toInt();
                String parse_count = "";
                extra_count++;
                parse_count.concat(extra_count);
                iot.configuration.set("c2_extra_amount_count", parse_count);
                cat_output = true;
            }
        }
        if (cat.equals("c3")) {
            if (checkFeedingLock("c3_lastfeedingtime", "c3_extra_delay",
                                 now.hour(), now.minute(), now.day()) &&
                checkFeedingExtra("c3_extra_amount_number",
                                  "c3_extra_amount_count")) {
                servo->rotate(
                    SERVO_ROTATE_FORWARD,
                    iot.configuration.get("c3_extra_amount_size").toInt());
                iot.configuration.set("c3_last_feedingtime",
                                      getFormattedDateTime());
                int extra_count =
                    iot.configuration.get("c3_extra_amount_count").toInt();
                String parse_count = "";
                extra_count++;
                parse_count.concat(extra_count);
                iot.configuration.set("c3_extra_amount_count", parse_count);
                cat_output = true;
            }
        }
    }
    if (time_output) {
        iot.configuration.set("last_feedingtime", getFormattedDateTime());
        iot.configuration.save();
    }
    if (cat_output) {

        iot.configuration.save();
    }
}

/**
 * @brief Checks the configured delay(timespan) of extra fooder.
 *
 * @param lf_time String to get the cat last feeding time.
 * @param extra_delay String to get the timespan while the cat has to wait.
 * @param hour The current hour as int.
 * @param minute The current minute as int.
 * @param day The current day of the month as int.
 *
 * @return true If the cat is not locked.
 * @return false Else.
 */
bool CNosh::checkFeedingLock(String lf_time, String extra_delay, int hour,
                             int minute, int day) {
    String lft = iot.configuration.get(lf_time);

    if (lft.equalsIgnoreCase(""))
        return false;

    int splitT = lft.indexOf("T");
    String dayStamp = lft.substring(0, splitT);
    String timeStamp = lft.substring(splitT + 1, lft.length() - 1);

    int splitTime = timeStamp.indexOf(":");
    int day_lft =
        dayStamp.substring(dayStamp.length() - 2, dayStamp.length()).toInt();

    if ((day - day_lft) >= 1) {
        // timespan more then one day
        return true;
    } else {
        int delay = iot.configuration.get(extra_delay).toInt();
        int hour_lft = timeStamp.substring(0, splitTime).toInt();
        int minute_lft =
            timeStamp.substring(splitTime + 1, splitTime + 3).toInt();

        // timespan less then one day
        if ((hour - hour_lft) >= 1) {
            // timespan more than one hour
            if ((hour - hour_lft) >= 2)
                return true;
            if ((hour - hour_lft) == 1) {
                if (delay == 30 || delay == 60)
                    return true;
                if ((minute - minute_lft) >= 30 && delay == 90)
                    return true;
            }
        } else {
            // timespan less than one hour
            if ((minute - minute_lft) >= 30 && delay == 30)
                return true;
            else
                return false;
        }
    }
}

/**
 * @brief Checks the count variable of extra fooder.
 *
 * @param number String to get the cat extra number.
 * @param count String to get the cat extra count.
 * @return true If the is more extra fooder avalible.
 * @return false If the extra fooder is used.
 */
bool CNosh::checkFeedingExtra(String number, String count) {
    if (iot.configuration.get(number).toInt() -
            iot.configuration.get(count).toInt() ==
        0)
        return false;
    else
        return true;
}

void CNosh::checkNextDay() {
    DateTime now = rtc.now();
    String datetime = "";
}

/**
 * @brief Checks the state of cnosh and prints the respective information on
 * the display every 3 seconds \n
 * State-1 : Accesspoint-Mode -> print IP and accesspointsecret \n
 * State-2: WLAN-Error -> print WLAN ERROR and print IP and
 * accesspointsecret \n
 * State-3 : Running -> print Filllevel
 *
 */
void CNosh::printLCD() {
    if (iot.configuration.get(ConfigurationKey::wifiConfigured)
            .equalsIgnoreCase("False")) {
        lcd->clear();
        lcd->printLine("IP: 192.168.4.1", 0);
        lcd->printLine(
            iot.configuration.get(ConfigurationKey::accessPointSecret), 1);
        vTaskDelay(3000);
    } else if (WiFi.status() != WL_CONNECTED) {
        lcd->clear();
        lcd->printLine("WLAN ERR-AP-MODE ", 0);
        lcd->printLine(
            iot.configuration.get(ConfigurationKey::accessPointSecret), 1);
        vTaskDelay(3000);
        lcd->clear();
        lcd->printLine("IP: 192.168.4.1", 0);
        lcd->printLine(
            iot.configuration.get(ConfigurationKey::accessPointSecret), 1);
        vTaskDelay(3000);
    } else {
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
        vTaskDelay(3000);
    }
}

/**
 * @brief Gets the current date and time from the real time clock.
 *
 * @return String A formatted DateTime-String("YYY-MM-DDTHH:MM:SSZ");
 */
String CNosh::getFormattedDateTime() {
    DateTime now = rtc.now();
    String datetime = "";
    datetime.concat(now.year());
    datetime.concat("-");
    if (now.month() < 10) {
        datetime.concat("0");
        datetime.concat(now.month());
    } else {
        datetime.concat(now.month());
    }
    datetime.concat("-");
    if (now.day() < 10) {
        datetime.concat("0");
        datetime.concat(now.day());
    } else {
        datetime.concat(now.day());
    }
    datetime.concat("T");
    if (now.hour() < 10) {
        datetime.concat("0");
        datetime.concat(now.hour());
    } else {
        datetime.concat(now.hour());
    }
    datetime.concat(":");
    if (now.minute() < 10) {
        datetime.concat("0");
        datetime.concat(now.minute());
    } else {
        datetime.concat(now.minute());
    }
    datetime.concat(":");
    datetime.concat("00Z");
    return datetime;
}

/**
 * @brief Creates the default configuration data and saves it.
 * Creats the FreeRTOS-tasks
 *
 */
void CNosh::initConfiguration() {
    iot.configuration.load();
    if (!iot.configuration.get(ConfigurationKey::cnoshConfiguration)
             .equalsIgnoreCase("1")) {
        iot.configuration.set(ConfigurationKey::time_1_h, "9");
        iot.configuration.set(ConfigurationKey::time_1_m, "00");
        iot.configuration.set(ConfigurationKey::time_2_h, "12");
        iot.configuration.set(ConfigurationKey::time_2_m, "00");
        iot.configuration.set(ConfigurationKey::time_3_h, "15");
        iot.configuration.set(ConfigurationKey::time_3_m, "00");
        iot.configuration.set(ConfigurationKey::time_4_h, "18");
        iot.configuration.set(ConfigurationKey::time_4_m, "00");
        iot.configuration.set(ConfigurationKey::time_amount_size, "1");

        iot.configuration.set(ConfigurationKey::c1_name, "");
        iot.configuration.set(ConfigurationKey::c1_uid, "");
        iot.configuration.set(ConfigurationKey::c1_lastfeedingtime, "");
        iot.configuration.set(ConfigurationKey::c1_extra_amount_size, "1");
        iot.configuration.set(ConfigurationKey::c1_extra_amount_number, "0");
        iot.configuration.set(ConfigurationKey::c1_extra_amount_count, "0");
        iot.configuration.set(ConfigurationKey::c1_extra_delay, "60");
        iot.configuration.set(ConfigurationKey::c1_created, "0");

        iot.configuration.set(ConfigurationKey::c2_name, "");
        iot.configuration.set(ConfigurationKey::c2_uid, "");
        iot.configuration.set(ConfigurationKey::c2_lastfeedingtime, "");
        iot.configuration.set(ConfigurationKey::c2_extra_amount_size, "1");
        iot.configuration.set(ConfigurationKey::c2_extra_amount_number, "0");
        iot.configuration.set(ConfigurationKey::c2_extra_amount_count, "0");
        iot.configuration.set(ConfigurationKey::c2_extra_delay, "60");
        iot.configuration.set(ConfigurationKey::c2_created, "0");

        iot.configuration.set(ConfigurationKey::c3_name, "");
        iot.configuration.set(ConfigurationKey::c3_uid, "");
        iot.configuration.set(ConfigurationKey::c3_lastfeedingtime, "");
        iot.configuration.set(ConfigurationKey::c3_extra_amount_size, "1");
        iot.configuration.set(ConfigurationKey::c3_extra_amount_number, "0");
        iot.configuration.set(ConfigurationKey::c3_extra_amount_count, "0");
        iot.configuration.set(ConfigurationKey::c3_extra_delay, "60");
        iot.configuration.set(ConfigurationKey::c3_created, "0");

        iot.configuration.set(ConfigurationKey::startdate,
                              getFormattedDateTime());

        iot.configuration.set(ConfigurationKey::total_amount_time, "0");
        iot.configuration.set(ConfigurationKey::total_amount_extra, "0");
        iot.configuration.set(ConfigurationKey::cnoshConfiguration, "1");

        iot.configuration.save();
    }

    //iot.configuration.dump();

    xTaskCreate(this->startTaskCNosh, "CNosh", 2048, this, 2, &cnosh_handle);
    xTaskCreate(this->startTaskButton, "Button", 2048, servo, 1,
               &button_handle);
    //xTaskCreate(this->startTaskLCD, "LCD", 8192, this, 1, NULL);
}

/**
 * @brief Creates all needed webserver routes that need to have access to the
 * cnosh components.
 *
 * @param config Takes the Configuration-object of Basecamp to access the data
 */
void CNosh::initWebserver(Configuration config) {
    iot.web.server.on(
        "/cnosh.json", HTTP_GET,
        [&config, this](AsyncWebServerRequest *request) {
            AsyncJsonResponse *response = new AsyncJsonResponse();
            DynamicJsonBuffer _jsonBuffer;

            JsonObject &_jsonData = response->getRoot();
            JsonObject &cats = _jsonData.createNestedObject("cats");

            cats["c1_name"] =
                _jsonBuffer.strdup(iot.configuration.get("c1_name"));
            cats["c1_uid"] =
                _jsonBuffer.strdup(iot.configuration.get("c1_uid"));
            cats["c1_lastfeedingtime"] =
                _jsonBuffer.strdup(iot.configuration.get("c1_lastfeedingtime"));
            cats["c1_extra_amount_size"] = _jsonBuffer.strdup(
                iot.configuration.get("c1_extra_amount_size"));
            cats["c1_extra_amount_number"] = _jsonBuffer.strdup(
                iot.configuration.get("c1_extra_amount_number"));
            cats["c1_extra_amount_count"] = _jsonBuffer.strdup(
                iot.configuration.get("c1_extra_amount_count"));
            cats["c1_extra_delay"] =
                _jsonBuffer.strdup(iot.configuration.get("c1_extra_delay"));
            cats["c1_created"] =
                _jsonBuffer.strdup(iot.configuration.get("c1_created"));
            cats["c2_name"] =
                _jsonBuffer.strdup(iot.configuration.get("c2_name"));
            cats["c2_uid"] =
                _jsonBuffer.strdup(iot.configuration.get("c2_uid"));
            cats["c2_lastfeedingtime"] =
                _jsonBuffer.strdup(iot.configuration.get("c2_lastfeedingtime"));
            cats["c2_extra_amount_size"] = _jsonBuffer.strdup(
                iot.configuration.get("c2_extra_amount_size"));
            cats["c2_extra_amount_number"] = _jsonBuffer.strdup(
                iot.configuration.get("c2_extra_amount_number"));
            cats["c2_extra_amount_count"] = _jsonBuffer.strdup(
                iot.configuration.get("c2_extra_amount_count"));
            cats["c2_extra_delay"] =
                _jsonBuffer.strdup(iot.configuration.get("c2_extra_delay"));
            cats["c2_created"] =
                _jsonBuffer.strdup(iot.configuration.get("c2_created"));
            cats["c3_name"] =
                _jsonBuffer.strdup(iot.configuration.get("c3_name"));
            cats["c3_uid"] =
                _jsonBuffer.strdup(iot.configuration.get("c3_uid"));
            cats["c3_lastfeedingtime"] =
                _jsonBuffer.strdup(iot.configuration.get("c3_lastfeedingtime"));
            cats["c3_extra_amount_size"] = _jsonBuffer.strdup(
                iot.configuration.get("c3_extra_amount_size"));
            cats["c3_extra_amount_number"] = _jsonBuffer.strdup(
                iot.configuration.get("c3_extra_amount_number"));
            cats["c3_extra_amount_count"] = _jsonBuffer.strdup(
                iot.configuration.get("c3_extra_amount_count"));
            cats["c3_extra_delay"] =
                _jsonBuffer.strdup(iot.configuration.get("c3_extra_delay"));
            cats["c3_created"] =
                _jsonBuffer.strdup(iot.configuration.get("c3_created"));

            JsonObject &feedingtimes =
                _jsonData.createNestedObject("feedingtimes");
            feedingtimes["time_1_h"] =
                _jsonBuffer.strdup(iot.configuration.get("time_1_h"));
            feedingtimes["time_1_m"] =
                _jsonBuffer.strdup(iot.configuration.get("time_1_m"));
            feedingtimes["time_2_h"] =
                _jsonBuffer.strdup(iot.configuration.get("time_2_h"));
            feedingtimes["time_2_m"] =
                _jsonBuffer.strdup(iot.configuration.get("time_2_m"));
            feedingtimes["time_3_h"] =
                _jsonBuffer.strdup(iot.configuration.get("time_3_h"));
            feedingtimes["time_3_m"] =
                _jsonBuffer.strdup(iot.configuration.get("time_3_m"));
            feedingtimes["time_4_h"] =
                _jsonBuffer.strdup(iot.configuration.get("time_4_h"));
            feedingtimes["time_4_m"] =
                _jsonBuffer.strdup(iot.configuration.get("time_4_m"));
            feedingtimes["time_amount_size"] =
                _jsonBuffer.strdup(iot.configuration.get("time_amount_size"));

            JsonObject &statistics = _jsonData.createNestedObject("statistics");
            statistics["configured"] =
                _jsonBuffer.strdup(iot.configuration.get("cnoshConfiguration"));
            statistics["startdate"] =
                _jsonBuffer.strdup(iot.configuration.get("startdate"));
            statistics["last_savedate"] =
                _jsonBuffer.strdup(iot.configuration.get("last_savedate"));
            statistics["last_feedingtime"] =
                _jsonBuffer.strdup(iot.configuration.get("last_feedingtime"));
            statistics["total_amount_time"] =
                _jsonBuffer.strdup(iot.configuration.get("total_amount_time"));
            statistics["total_amount_extra"] =
                _jsonBuffer.strdup(iot.configuration.get("total_amount_extra"));

            response->setLength();
            // NOTE: AsyncServer.send(ptr* foo) deletes `response` after async
            // send. As this is not documented in the header there: thanks for
            // nothing.
            request->send(response);
        });
    iot.web.server.on(
        "/cats.json", HTTP_GET,
        [&config, this](AsyncWebServerRequest *request) {
            AsyncJsonResponse *response = new AsyncJsonResponse();
            DynamicJsonBuffer _jsonBuffer;

            JsonObject &_jsonData = response->getRoot();
            JsonObject &cats = _jsonData.createNestedObject("cats");

            cats["c1_name"] =
                _jsonBuffer.strdup(iot.configuration.get("c1_name"));
            cats["c1_uid"] =
                _jsonBuffer.strdup(iot.configuration.get("c1_uid"));
            cats["c1_lastfeedingtime"] =
                _jsonBuffer.strdup(iot.configuration.get("c1_lastfeedingtime"));
            cats["c1_extra_amount_size"] = _jsonBuffer.strdup(
                iot.configuration.get("c1_extra_amount_size"));
            cats["c1_extra_amount_number"] = _jsonBuffer.strdup(
                iot.configuration.get("c1_extra_amount_number"));
            cats["c1_extra_amount_count"] = _jsonBuffer.strdup(
                iot.configuration.get("c1_extra_amount_count"));
            cats["c1_extra_delay"] =
                _jsonBuffer.strdup(iot.configuration.get("c1_extra_delay"));
            cats["c1_created"] =
                _jsonBuffer.strdup(iot.configuration.get("c1_created"));
            cats["c2_name"] =
                _jsonBuffer.strdup(iot.configuration.get("c2_name"));
            cats["c2_uid"] =
                _jsonBuffer.strdup(iot.configuration.get("c2_uid"));
            cats["c2_lastfeedingtime"] =
                _jsonBuffer.strdup(iot.configuration.get("c2_lastfeedingtime"));
            cats["c2_extra_amount_size"] = _jsonBuffer.strdup(
                iot.configuration.get("c2_extra_amount_size"));
            cats["c2_extra_amount_number"] = _jsonBuffer.strdup(
                iot.configuration.get("c2_extra_amount_number"));
            cats["c2_extra_amount_count"] = _jsonBuffer.strdup(
                iot.configuration.get("c2_extra_amount_count"));
            cats["c2_extra_delay"] =
                _jsonBuffer.strdup(iot.configuration.get("c2_extra_delay"));
            cats["c2_created"] =
                _jsonBuffer.strdup(iot.configuration.get("c2_created"));
            cats["c3_name"] =
                _jsonBuffer.strdup(iot.configuration.get("c3_name"));
            cats["c3_uid"] =
                _jsonBuffer.strdup(iot.configuration.get("c3_uid"));
            cats["c3_lastfeedingtime"] =
                _jsonBuffer.strdup(iot.configuration.get("c3_lastfeedingtime"));
            cats["c3_extra_amount_size"] = _jsonBuffer.strdup(
                iot.configuration.get("c3_extra_amount_size"));
            cats["c3_extra_amount_number"] = _jsonBuffer.strdup(
                iot.configuration.get("c3_extra_amount_number"));
            cats["c3_extra_amount_count"] = _jsonBuffer.strdup(
                iot.configuration.get("c3_extra_amount_count"));
            cats["c3_extra_delay"] =
                _jsonBuffer.strdup(iot.configuration.get("c3_extra_delay"));
            cats["c3_created"] =
                _jsonBuffer.strdup(iot.configuration.get("c3_created"));

            response->setLength();
            // NOTE: AsyncServer.send(ptr* foo) deletes `response` after async
            // send. As this is not documented in the header there: thanks for
            // nothing.
            request->send(response);
        });
    iot.web.server.on(
        "/feedingtimes.json", HTTP_GET,
        [&config, this](AsyncWebServerRequest *request) {
            AsyncJsonResponse *response = new AsyncJsonResponse();
            DynamicJsonBuffer _jsonBuffer;

            JsonObject &_jsonData = response->getRoot();

            JsonObject &feedingtimes =
                _jsonData.createNestedObject("feedingtimes");
            feedingtimes["time_1_h"] =
                _jsonBuffer.strdup(iot.configuration.get("time_1_h"));
            feedingtimes["time_1_m"] =
                _jsonBuffer.strdup(iot.configuration.get("time_1_m"));
            feedingtimes["time_2_h"] =
                _jsonBuffer.strdup(iot.configuration.get("time_2_h"));
            feedingtimes["time_2_m"] =
                _jsonBuffer.strdup(iot.configuration.get("time_2_m"));
            feedingtimes["time_3_h"] =
                _jsonBuffer.strdup(iot.configuration.get("time_3_h"));
            feedingtimes["time_3_m"] =
                _jsonBuffer.strdup(iot.configuration.get("time_3_m"));
            feedingtimes["time_4_h"] =
                _jsonBuffer.strdup(iot.configuration.get("time_4_h"));
            feedingtimes["time_4_m"] =
                _jsonBuffer.strdup(iot.configuration.get("time_4_m"));
            feedingtimes["time_amount_size"] =
                _jsonBuffer.strdup(iot.configuration.get("time_amount_size"));

            response->setLength();
            // NOTE: AsyncServer.send(ptr* foo) deletes `response` after async
            // send. As this is not documented in the header there: thanks for
            // nothing.
            request->send(response);
        });
    iot.web.server.on(
        "/statistics.json", HTTP_GET,
        [&config, this](AsyncWebServerRequest *request) {
            AsyncJsonResponse *response = new AsyncJsonResponse();
            DynamicJsonBuffer _jsonBuffer;

            JsonObject &_jsonData = response->getRoot();

            JsonObject &statistics = _jsonData.createNestedObject("statistics");
            statistics["configured"] =
                _jsonBuffer.strdup(iot.configuration.get("cnoshConfiguration"));
            statistics["startdate"] =
                _jsonBuffer.strdup(iot.configuration.get("startdate"));
            statistics["last_savedate"] =
                _jsonBuffer.strdup(iot.configuration.get("last_savedate"));
            statistics["last_feedingtime"] =
                _jsonBuffer.strdup(iot.configuration.get("last_feedingtime"));
            statistics["total_amount_time"] =
                _jsonBuffer.strdup(iot.configuration.get("total_amount_time"));
            statistics["total_amount_extra"] =
                _jsonBuffer.strdup(iot.configuration.get("total_amount_extra"));

            response->setLength();
            // NOTE: AsyncServer.send(ptr* foo) deletes `response` after async
            // send. As this is not documented in the header there: thanks for
            // nothing.
            request->send(response);
        });
    iot.web.server.on(
        "/delete_cat", HTTP_POST,
        [&config, this](AsyncWebServerRequest *request) {
            if (request->params() == 0) {
                DEBUG_PRINTLN("Refusing to take over an empty "
                              "configuration submission.");
                request->send(500);
                return;
            }

            AsyncWebParameter *webParameter = request->getParam(0);
            if (webParameter->isPost() && webParameter->value().length() != 0) {
                Serial.print("Katze: ");
                Serial.print(webParameter->name());
                Serial.print(" soll gelöscht werden");

                String cat = webParameter->name();
                if (cat.equalsIgnoreCase("c1_uid")) {
                    iot.configuration.set("c1_name", "");
                    iot.configuration.set("c1_uid", "");
                    iot.configuration.set("c1_lastfeedingtime", "");
                    iot.configuration.set("c1_extra_amount_number", "0");
                    iot.configuration.set("c1_extra_amount_size", "1");
                    iot.configuration.set("c1_extra_amount_count", "0");
                    iot.configuration.set("c1_extra_delay", "60");
                    iot.configuration.set("c1_created", "0");
                }
                if (cat.equalsIgnoreCase("c2_uid")) {
                    iot.configuration.set("c2_name", "");
                    iot.configuration.set("c2_uid", "");
                    iot.configuration.set("c2_lastfeedingtime", "");
                    iot.configuration.set("c2_extra_amount_number", "0");
                    iot.configuration.set("c2_extra_amount_size", "1");
                    iot.configuration.set("c2_extra_amount_count", "0");
                    iot.configuration.set("c2_extra_delay", "60");
                    iot.configuration.set("c2_created", "0");
                }
                if (cat.equalsIgnoreCase("c3_uid")) {
                    iot.configuration.set("c3_name", "");
                    iot.configuration.set("c3_uid", "");
                    iot.configuration.set("c3_lastfeedingtime", "");
                    iot.configuration.set("c3_extra_amount_number", "0");
                    iot.configuration.set("c3_extra_amount_size", "1");
                    iot.configuration.set("c3_extra_amount_count", "0");
                    iot.configuration.set("c3_extra_delay", "60");
                    iot.configuration.set("c3_created", "0");
                }
            }

            iot.configuration.save();
            request->send(201);
        });
    iot.web.server.on(
        "/add_cat", HTTP_POST, [&config, this](AsyncWebServerRequest *request) {
            if (request->params() == 0) {
                DEBUG_PRINTLN("Refusing to take over an empty "
                              "configuration submission.");
                request->send(500);
                return;
            }

            AsyncWebParameter *webParameter = request->getParam(0);
            if (webParameter->isPost() && webParameter->value().length() != 0) {
                Serial.print("Katze: ");
                Serial.print(webParameter->name());
                Serial.print(" soll hinzugefügt werden.");

                String cat = webParameter->name();
                if (cat.equalsIgnoreCase("c1_name")) {
                    iot.configuration.set("c1_name",
                                          webParameter->value().c_str());
                    iot.configuration.set("c1_created", "1");
                }
                if (cat.equalsIgnoreCase("c2_name")) {
                    iot.configuration.set("c2_name",
                                          webParameter->value().c_str());
                    iot.configuration.set("c2_created", "1");
                }
                if (cat.equalsIgnoreCase("c3_name")) {
                    iot.configuration.set("c3_name",
                                          webParameter->value().c_str());
                    iot.configuration.set("c3_created", "1");
                }
            }

            iot.configuration.save();
            request->send(201);
        });
    iot.web.server.on(
        "/submitfeedingtime", HTTP_POST,
        [&config, this](AsyncWebServerRequest *request) {
            if (request->params() == 0) {
                DEBUG_PRINTLN(
                    "Refusing to take over an empty configuration submission.");
                request->send(500);
                return;
            }

            for (int i = 0; i < request->params(); i++) {
                AsyncWebParameter *webParameter = request->getParam(i);
                if (webParameter->isPost() &&
                    webParameter->value().length() != 0) {
                    iot.configuration.set(webParameter->name().c_str(),
                                          webParameter->value().c_str());
                }
            }

            iot.configuration.save();
            request->send(201);
        });
    iot.web.server.on(
        "/set_amount", HTTP_POST,
        [&config, this](AsyncWebServerRequest *request) {
            if (request->params() == 0) {
                DEBUG_PRINTLN(
                    "Refusing to take over an empty configuration submission.");
                request->send(500);
                return;
            }

            for (int i = 0; i < request->params(); i++) {
                AsyncWebParameter *webParameter = request->getParam(i);
                if (webParameter->isPost() &&
                    webParameter->value().length() != 0) {
                    iot.configuration.set(webParameter->name().c_str(),
                                          webParameter->value().c_str());
                }
            }

            iot.configuration.save();
            request->send(201);
        });
    iot.web.server.on(
        "/set_rfid", HTTP_POST,
        [&config, this](AsyncWebServerRequest *request) {
            if (request->params() == 0) {
                DEBUG_PRINTLN(
                    "Refusing to take over an empty configuration submission.");
                request->send(500);
                return;
            }

            AsyncWebParameter *webParameter = request->getParam(0);
            if (webParameter->isPost() && webParameter->value().length() != 0) {
                iot.configuration.set(webParameter->name().c_str(),
                                      webParameter->value().c_str());
            }

            iot.configuration.save();
            request->send(201);
        });
    iot.web.server.on("/reset_system", HTTP_POST,
                      [&config, this](AsyncWebServerRequest *request) {
                          Serial.println("Factory reset was forced.");
                          // Format the flash storage
                          SPIFFS.format();
                          // Reset the boot counter
                          iot.preferences.putUInt("bootcounter", 0);
                          // Call the destructor for preferences so that all
                          // data is safely stored befor rebooting
                          iot.preferences.end();
                          // send success
                          request->send(201);

                          Serial.println("Rebooting.");
                          // Reboot
                          ESP.restart();
                      });
    iot.web.server.on("/reset_statistics", HTTP_POST,
                      [&config, this](AsyncWebServerRequest *request) {
                          iot.configuration.set("last_feedingtime", "");
                          iot.configuration.set("total_amount_time", "0");
                          iot.configuration.set("total_amount_extra", "0");

                          request->send(201);
                      });
    iot.web.server.on("/search_rfid", HTTP_POST,
                      [&config, this](AsyncWebServerRequest *request) {
                          bool rfid_unit = false;

                          unsigned long start = millis();
                          unsigned long duration = 6000;

                          while (millis() <= start + duration) {
                              if (rfid->detectUnit()) {
                                  String uid = rfid->getUidAsString();
                                  String text = "{\"uid\": ";
                                  text.concat("\"");
                                  text.concat(uid);
                                  text.concat("\"}");
                                  request->send(200, "application/json", text);
                                  rfid_unit = true;
                                  break;
                              }
                          }

                          if (!rfid_unit) {
                              request->send(500);
                          }
                      });
}