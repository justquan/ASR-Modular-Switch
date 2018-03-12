//Does NOT work when using ftdi breakout because LED flashes on and off on it, changing Analog value. not fully verified working yet 3/4
//Note: When looking at analog sound values, a lower number means higher volume.
//When using sound modules like the KY-038, the analog value changes when there are other energy consuming components connected,like LEDS.
//Note: strobe not implemented with sound sensor, which toggles relay state

//TODO: instead of delays, possibly use timeElapsed for clap detection
//receives the three char command from the interpretCommand(), and makes sense of it
void volumeInterpret(String filteredData) {
  if (filteredData.substring(1).equals ("1CL")) {
    useOneClap = true;
    useTwoClaps = false;
    Serial.println("Set to 1 clap");
  }
  else if (filteredData.substring(1).equals("2CL")) {
    useOneClap = false;
    useTwoClaps = true;
    Serial.println("Set to 2 claps");

  }
  else {
    //assuming it is an integer 1-20
    soundDifference = (int)convertCommandToLong(filteredData);
    if (DEBUG) {
      Serial.print("soundDifference = ");
      Serial.println(soundDifference);
    }
  }
}

void soundSwitch() {
  if (firstSensorCall) {
    openRelay();//IMPORTANT: make sure relay is open / off, so that it is not consuming power and interfering with the sound sensor analog values
    delay(setupDelay);//time for relay to open, if not open already
    setNormalVolume();
    firstSensorCall = false;
  }
  if (useOneClap) {
    oneClapToggle(); //TODO: Make it so that user can select one clap toggle or twoClapToggle.
  }
  else if (useTwoClaps) {
    twoClapToggle();
  }
}

void oneClapToggle() {
  int currentVolume = getVolumeAnalog();
  errorDetection(currentVolume);
  if (normalVolume - currentVolume >= soundDifference) { //If the currentvolume has a volume lower than the normal volume by aoundDifference units, it is triggered.
    if (DEBUG) {
      Serial.print("1st triggering sound val: ");
      Serial.println(currentVolume);
    }
    reverseRelay();//toggle relay
    if (DEBUG) {
      Serial.print("Triggered. Delay for ");
      Serial.println(soundTriggerDelay);
    }
    delay(soundTriggerDelay);//TODO: Use millis() timer instead of delay(), because delay() delays all other processes in addition to soundSwitch
    //delay is to avoid debouncing, the switch shouldn't listen to data right after it receives triggering data
  }
}
//  boolean oddTime = true;//for debugging

void twoClapToggle() {//TODO: juse timeelapsed instead of millis()
  boolean triggered = false;
  int firstVolume = getVolumeAnalog();//includes offset for relay if needed
  errorDetection(firstVolume);
  if (normalVolume - firstVolume >= soundDifference) {
    if (DEBUG) {
      Serial.print("1st peak sound val: ");
      Serial.println(firstVolume);
      Serial.print("Delay for ");
      Serial.println(twoClapWait1);
    }
    delay(twoClapWait1);
    if (abs(normalVolume - getVolumeAnalog()) <= 3) {//error of 3, meaning that it won't work if the user selects 3 as the sensitivity on the app
      if (DEBUG) {
        Serial.println("sound val in normal range");
        Serial.print("Delay for ");
        Serial.println(twoClapWait2);
      }
      long currentTime = millis();
      while (millis() - currentTime <= twoClapWait2 && !triggered) {
        int secondVolume = getVolumeAnalog();
        if (normalVolume - secondVolume >= soundDifference) {
          if (DEBUG) {
            Serial.print("2nd peak sound val: ");
            Serial.println(secondVolume);
          }
          reverseRelay();
          triggered = true;
          delay(soundTriggerDelay);
        }
      }
    }
  }
}

void setNormalVolume() {
  int totalNormalVolume = 0;
  int numNormalVolumes = 30;//Number of times the for loop is run through
  for (int i = 0; i < numNormalVolumes; i++) {
    totalNormalVolume += getVolumeAnalog();
  }
  normalVolume = totalNormalVolume / numNormalVolumes; //average normal volume
  if (DEBUG) {
    Serial.println("normalVolume = ");
    Serial.print(normalVolume);
    Serial.println();
  }
}

//returns current analog value from the sensor
int getVolumeAnalogRaw() {
  return analogRead(volPin);
}

int getVolumeAnalog() {
  int alteredValue = getVolumeAnalogRaw();
  if (relayClosed) {
    return alteredValue + relayAnalogValOffsetSound;
  }
  else {
    return alteredValue;
  }
}

//TODO: test this and find a place to put it. May have to make a separate function in the main loop that checks the module number while in setup mode, and uses the correct print function for the right sensor.
void btPrintSound() {
  String val = String(getVolumeAnalog());
  String msg = btSendBlock + val;//offset
  BT.println(msg);
}

void printSoundRaw() {
  Serial.println(getVolumeAnalogRaw());
}

