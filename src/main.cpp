#include <Arduino.h>
#include <CNosh.hpp>

CNosh *cnosh = new CNosh();

void setup() {
  Serial.begin(115200);
  cnosh->init();
}

void loop() {
  cnosh->begin();
}