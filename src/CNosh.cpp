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
    // for using slave i2c-Bus
    // don't need this line when using master
    Wire.begin(SDA_SLAVE, SCL_SLAVE);

    //iot.begin();
    pinMode(BUTTON_PIN, INPUT_PULLDOWN);
    lcd->init();
    rfid->init();
    
    this->initConfiguration();
    return true;
}

bool CNosh::begin()
{
    Serial.println("Hello World!");
    delay(3000);
    return true;
}

void CNosh::startTaskLCD(void *lcdObj)
{
    LCD * lcdTest = (LCD *) lcdObj;
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
    while (1)
    {
        // Serial.println("Printing LCD");
        // Serial.println("Filllevel: ");
        //Serial.print(measure->readDistance());
        vTaskDelay( xDelay );
        lcdTest->clear();
        lcdTest->printLine("Welcome to CNosh", 0);
        lcdTest->printLine("FillLevel: ", 1);
    }
}

void CNosh::startTaskButton(void *servoObj)
{
    int press1 = 0;
    ServoEngine *servoTest = (ServoEngine *) servoObj;
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
    RFID *rfidTest = (RFID *) rfidObj;
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
    while (1)
    {
        rfidTest->detectUnit();
        vTaskDelay( xDelay );
    }
}

void CNosh::startTasks()
{
    xTaskCreate(this->startTaskLCD, "LCD", 2048, lcd, 3, NULL);
    xTaskCreate(this->startTaskButton, "Button", 2048, servo, 1, NULL);
    xTaskCreate(this->startTaskRFID, "RFID", 2048, rfid, 5, NULL);
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