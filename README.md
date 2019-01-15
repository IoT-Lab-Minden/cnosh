# cnosh
**An intelligent cat food dispenser based on an ESP32-Microcontoller.**

In this repository the cnosh will be created by [Dario](https://github.com/dleunig) and [Alex](https://github.com/abergy) for the elective "Embedded-Software" on the FH-Bielefeld.

For a better overview we linked all diagrams and documents that we created in the [GitHub-Wiki](https://github.com/IoT-Lab-Minden/cnosh/wiki). Everything here is under development and will be updated regularly.

If there are any questions, please send us a mail:

- dleunig@fh-bielefeld.de
- alexander.bergmann@fh-bielefeld.de

## Getting Started

1. Make sure you can flash the ESP32 with the Arduino-IDE
2. Add the dependency libraries in Arduino-IDE
3. Put the following code in the `main.cpp` and flash it
4. Thats it! 

```c++
#include <CNosh.hpp>

CNosh *cnosh = new CNosh();

void setup() {
  Serial.begin(115200);
  cnosh->begin();
}

void loop() {}
```

## Documentation
[GitHub Pages](https://dleunig.github.io/cnosh/)

## Dependency libraries

- [Basecamp-cnosh](https://github.com/abergy/Basecamp-cnosh)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson/tree/master)
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP/tree/master)
- [ESPAsyncWebserver](https://github.com/me-no-dev/ESPAsyncWebServer/tree/master)
- [async-mqtt-client](https://github.com/marvinroger/async-mqtt-client/tree/master) (optional)
- [Adafruit_VL53L0X](https://github.com/adafruit/Adafruit_VL53L0X)
- [ESP32-Arduino-Servo](https://github.com/RoboticsBrno/ESP32-Arduino-Servo-Library)
- [NTPClient](https://github.com/arduino-libraries/NTPClient)
- [MFRC522](https://github.com/miguelbalboa/rfid)
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)

