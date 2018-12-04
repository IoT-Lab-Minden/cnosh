#include <Arduino.h>
#include <CNosh.hpp>

CNosh *cnosh = new CNosh();

void setup() {
  cnosh->init();
  
}

void loop() {
  cnosh->begin();
}