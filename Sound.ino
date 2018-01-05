//Note: When looking at analog sound values, a lower number means higher volume.

//receives the three char command from the interpretCommand(), and makes sense of it
void volumeInterpret(String filteredData) {
  //assuming it is an integer 1-10
  setNormalVolume();
  soundDifference = (int)convertCommandToLong(filteredData);
  if (DEBUG) {
    Serial.println("Sound Difference value is set to ");
    Serial.print(soundDifference);
    Serial.println();
  }
}
//TODO: implement two clap feature
void soundSwitch() {
  twoClapToggle();
}

void oneClapToggle() {
  int triggerDelay = 1000;//after triggering, how long it waits before sensing agian
  int currentVolume = getVolumeAnalog();
  if (normalVolume - currentVolume >= soundDifference) { //If the currentvolume has a volume lower than the normal volume by aoundDifference units, it is triggered.
    if (DEBUG) {
      Serial.println("The initial value of the sound that just triggered the relay is ");
      Serial.print(currentVolume);
      Serial.println();
    }
    reverseRelay();//toggle relay
    if (DEBUG) {
      Serial.println("Triggered. Delaying listening for ");
      Serial.print(triggerDelay);
      Serial.println();
    }
    delay(triggerDelay);//TODO: Use millis() timer instead of delay(), because delay() delays all other processes in addition to soundSwitch
    //delay is to avoid debouncing, the switch shouldn't listen to data right after it receives triggering data
  }
}

void twoClapToggle() {
  int firstWait = 150;
  int secondWait = 300;
  int triggerDelay = 1000;//after triggering, how long it waits before sensing agian
  int firstVolume = getVolumeAnalog();
  if (normalVolume - firstVolume >= soundDifference) {
    if (DEBUG) {
      Serial.println("The first peak value of the sound is ");
      Serial.print(firstVolume);
      Serial.println();
      Serial.println("Delaying for ");
      Serial.print(firstWait);
      Serial.println();
    }
    delay(firstWait);
    if (abs(normalVolume - getVolumeAnalog()) <= 1) {
      if (DEBUG) {
        Serial.println("The value of sound is within the range of normal.");
        Serial.println("Delaying for ");
        Serial.print(secondWait);
        Serial.println();
      }
      delay(secondWait);
      int secondVolume = getVolumeAnalog();
      if (normalVolume - secondVolume >= soundDifference) {
        if (DEBUG) {
          Serial.println("The second peak value of the sound is ");
          Serial.print(secondVolume);
          Serial.println();
        }
        reverseRelay();
      }
    }
  }
}

void setNormalVolume() {
  int totalNormalVolume = 0;
  int numNormalVolumes = 10;//Number of times the for loop is run through
  for (int i = 0; i < numNormalVolumes; i++) {
    totalNormalVolume += getVolumeAnalog();
  }
  normalVolume = totalNormalVolume / numNormalVolumes; //average normal volume
  if (DEBUG) {
    Serial.println("Normal Volume is set to ");
    Serial.print(normalVolume);
    Serial.println();
  }
}

//returns current analog value from the sensor
int getVolumeAnalog() {
  return analogRead(volPin);
}

