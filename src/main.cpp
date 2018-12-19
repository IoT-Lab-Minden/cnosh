#include <CNosh.hpp>

CNosh *cnosh = new CNosh();

MFRC522 mfrc522(SS_PIN, RST_PIN);
void setup() {
  Serial.begin(115200);
  
  while (!Serial); // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  cnosh->init();
  
}

void loop() {
  //   // Look for new cards
  // if ( ! mfrc522.PICC_IsNewCardPresent()) {
  // return;
  // }
   
  // // Select one of the cards
  // if ( ! mfrc522.PICC_ReadCardSerial()) {
  // return;
  // }
 
  // // Dump debug info about the card; PICC_HaltA() is automatically called
  // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  delay(2000);
}