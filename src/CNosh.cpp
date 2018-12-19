/*
   CNosh - Firmware for ESP32 based cat food dispenser with Wi-Fi control
   Written by 
   - Alexander Bergmann (alexander.bergmann@fh-bielefeld.de)
   - Dario Leunig (dleunig@fh-bielefeld.de)
   
   Licensed under GPLv3. See LICENSE for details.
   */

#include <CNosh.hpp>

CNosh::CNosh()
{
    settings = &jsonBuffer.createObject();
    lcd = new LCD();
    measure = new Measure();
    servo = new ServoEngine();
    rfid = new RFID();
}

bool CNosh::init()
{
    //iot.begin();
    this->initConfiguration();
    // for using slave i2c-Bus
    // don't need this line when using master
    Wire.begin(SDA_SLAVE, SCL_SLAVE);
    lcd->init();
    rfid->init();
    pinMode(BUTTON_PIN, INPUT_PULLDOWN);
    return true;
}

bool CNosh::begin()
{
    Serial.println("Hello World!");
    delay(3000);
    return true;
}

void CNosh::taskLCD()
{
    while (1)
    {
        Serial.println("Printing LCD");
        Serial.println("Filllevel: ");
        delay(2000);
        //Serial.print(measure->readDistance());

        // lcd->clear();
        // lcd->printLine("Welcome to CNosh", 0);
        // lcd->printLine("FillLevel: ", 1);
        //delay(2000);
    }
}
void CNosh::taskButton()
{
    int press1 = 0;
    while (1)
    {
        press1 = digitalRead(BUTTON_PIN);
        if (press1 == LOW)
        {
            //servo->stop();
            Serial.println("LOW");
            
        }
        else
        {
            //servo->rotate(180, 0);
            Serial.println("HIGH");
        }
        delay(2000);
    }
}
void CNosh::taskRFID()
{
    while (1)
    {
        delay(3000);
        Serial.println("RFID");
        //rfid->detectUnit();
        //rfid->dumpToSerial();
    }
}

void CNosh::startTaskLCD(void *lcdObj)
{
    //LCD * lcdTest = (ServoEngine *) servoObj;
    
}

void CNosh::startTaskButton(void *servoObj)
{
    int press1 = 0;
    ServoEngine * servoTest = (ServoEngine *) servoObj;
    while (1)
    {
        press1 = digitalRead(BUTTON_PIN);
        if (press1 == LOW)
        {
            servoTest->stop();
        }
        else
        {
            servoTest->rotate(180, 0);
        }
    }
}

void CNosh::startTaskRFID(void *rfidObj)
{
    RFID * rfidTest = (RFID *) rfidObj;

    while (1)
    {
        rfidTest->detectUnit();
        //rfidTest->readUnitSerial();
    }
}

void CNosh::startTasks()
{
    //xTaskCreate(this->startTaskLCD, "LCD", 2048, NULL, 5, NULL);
    xTaskCreate(this->startTaskButton, "Button", 2048, servo, 1, NULL);
    //xTaskCreate(this->startTaskRFID, "RFID", 2048, rfid, 1, NULL);
}

bool CNosh::initConfiguration()
{
/*     if (!iot.configuration.isKeySet(ConfigurationKey::cnoshConfiguration))
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
    startTasks();
    return true;
}