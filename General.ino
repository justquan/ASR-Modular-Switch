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
    if (DEBUG) {
      Serial.println("strobeIfTriggered = true.");
    }
  }
  else if (filteredData.equals("NSB")) {//if receives no strobe command, then set strobeIfTriggered to false
    strobeIfTriggered = false; // TODO: properly test and implement
    Serial.println("strobeIfTriggered = false.");
  }
  else if (filteredData.equals("DCT")) {
    btPowerOff();
    delay(btDisconnectDelay);//gives time to turn off to prevent interference in analog signals. Not affected by clock slowing, so no need to scale.
    setupSwitch = false;
    if (DEBUG) {
      Serial.println("Disconnecting");
    }
    statusLEDOff();
    //    setClockSpeedSlow();
  }
  else if (filteredData.equals("SLP")) { //TODO: Make a sleep button in Android app
    delay(50);
    sleep();
  }
  else {//if receiving a number, uses it for the timer
    generalTimerWait = convertCommandToLong(filteredData) * generalTimerMillisPerUnit;//sets the timer wait to equal the value received multiplied by generalTimerMillisPerUnit.
    timeElapsedGeneralTimerWait = 0;//starts counting down timer before triggering at this point
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

//initial setting to default clock speed
void setClockSpeedDefault() {
  CLKPR = 0x80;//required protocol to set the clock frequency
  CLKPR = 0x00;//sets the 16 MHz clock frequency to a 1:1 default, unchanged ratio.
  if (DEBUG) {
    Serial.println("Clock speed set to normal. ");
  }
}

//called after BT disconnected and setupSwitch is false, running the normal sensorChooser(). Slows clock speed to save power.
void setClockSpeedSlow() {
  CLKPR = 0x80;//required protocol to set the clock frequency
  CLKPR = clockSpeedReduction;//sets the 16 MHz clock frequency to half speed. 0x00 = unchanged, 0x01 = half speed, 0x02 = quarter speed, 0x03 = eigth speed, etc.
}

//uses library fuctions to put microcontroller to sleep
void sleep() {
  openRelay();
  if (DEBUG) {
    Serial.println("Sleep");
  }
  delay(100); //time for the print and for the relay to open if not already
  btPowerOff();
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
  delay(1000);
}

//checks to see if the amount of time since switch was turned on is over or at the minimum time before going to sleep, and if it is and is in setup mode, calls sleep()
//the check for btConnectionMade is to make sure it doesn't go to sleep while a user is trying to connect to it.
void checkSleep() {
  if (millis() >= minTimeBeforeSleep && setupSwitch && !btConnectionMade) {
    sleep();
  }
}

void generalTimerSwitch() {
  if (generalTimerWait != 0) {
    if (timeElapsedGeneralTimerWait >= generalTimerWait) {
      trigger();
    }
  }
}
