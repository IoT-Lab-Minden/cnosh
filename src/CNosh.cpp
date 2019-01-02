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
    pinMode(BUTTON_PIN, INPUT_PULLDOWN);
    lcd->init();
    // for using slave i2c-Bus
    // don't need this line when using master
    Wire.begin(SDA_SLAVE, SCL_SLAVE);
    
    
    
    //rfid->init();
    
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
    while (1)
    {
        Serial.println("Printing LCD");
        Serial.println("Filllevel: ");
        delay(2000);
        //Serial.print(measure->readDistance());

        lcdTest->clear();
        lcdTest->printLine("Welcome to CNosh", 0);
        lcdTest->printLine("FillLevel: ", 1);
        delay(2000);
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
    MFRC522 *rfidTest = (MFRC522 *) rfidObj;

    while (1)
    {
        // Look for new cards
        if ( ! rfidTest->PICC_IsNewCardPresent()) {
        return;
        }
        
        // Select one of the cards
        if ( ! rfidTest->PICC_ReadCardSerial()) {
        return;
        }
        
        // Dump debug info about the card; PICC_HaltA() is automatically called
        rfidTest->PICC_DumpToSerial(&(rfidTest->uid));
    }
}

void CNosh::startTasks()
{
    xTaskCreate(this->startTaskLCD, "LCD", 2048, lcd, 5, NULL);
    //xTaskCreate(this->startTaskButton, "Button", 2048, servo, 1, NULL);
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