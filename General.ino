void generalInterpret(String filteredData) {
  if (filteredData.equals("OFF")) {
    openRelay();
  }
  else if (filteredData.equals("_ON")) {
    closeRelay();
  }
  else if (filteredData.equals("T01")) {
    triggerStateIsClose = true;  //sets triggerStateClosed to true, so when triggered, it closes the relay
  }
  else if (filteredData.equals("T00")) {
    triggerStateIsClose = false;//sets triggerStateClosed to false, so when triggered, it opens the relay
  }
  else if (filteredData.equals("YSB")) {
    strobeIfTriggered = true;//if receives yes strobe command, then set strobeIfTriggered to true
  }
  else if (filteredData.equals("NSB")) {//if receives no strobe command, then set strobeIfTriggered to false
    strobeIfTriggered = false; // TODO: properly test and implement
  }
  else if (filteredData.equals("DCT")) {
    btPowerOff();
    delay(1000);//gives time to turn off to prevent interference in analog signals
    setupSwitch = false;
  }
}

//disconnect GND to HC-06 bt module by sending no power to the base pin of the NPN transistor
void btPowerOff() {
  digitalWrite(btPowerBasePin, LOW);
}

//Connect GND to HC-06 bt module by sending power to the base pin of the NPN transistor
void btPowerOn() {
  digitalWrite(btPowerBasePin, HIGH);
}

void redStatusLEDOn() {
  digitalWrite(redStatusLED, HIGH);
  digitalWrite(greenStatusLED, LOW);
}

void greenStatusLEDOn() {
  digitalWrite(greenStatusLED, HIGH);
  digitalWrite(redStatusLED, LOW);
}

void statusLEDOff() {
  digitalWrite(redStatusLED, LOW);
  digitalWrite(greenStatusLED, LOW);
}

void clearEEPROM() { //for testing, to clear all EEPROM data
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

//prints every EEPROM Byte, including zero values
void printAllEEPROMBytes() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    Serial.println(EEPROM.read(i));
  }
}

//prints all longs stored with EEPROM_writeAnything(), but also includes zero values.
void printAllEEPROMAnything() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    long temp;
    EEPROM_readAnything(i, temp);
    Serial.println(temp);
  }
}

