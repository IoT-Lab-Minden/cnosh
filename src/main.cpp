#include <CNosh.hpp>

CNosh *cnosh = new CNosh();

void setup() {
  Serial.begin(115200);
  cnosh->init();
}

void loop() {
  //cnosh->begin();
  //xTaskCreate(&cnosh->begin, "cnosh_test", 2048, NULL, 5, NULL);
}