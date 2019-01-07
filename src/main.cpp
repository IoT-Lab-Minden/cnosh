#include <CNosh.hpp>

CNosh *cnosh = new CNosh();

// MFRC522 mfrc522(SS_PIN, RST_PIN);
void setup() {
    Serial.begin(115200);
    cnosh->init();
}

void loop() { delay(1000); }