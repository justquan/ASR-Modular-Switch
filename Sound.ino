//Note: When looking at analog sound values, a lower number means higher volume.
//When using sound modules like the KY-038, the analog value changes when there are other energy consuming components connected,like LEDS.

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
  oneClapToggle();
  //twoClapToggle();
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
  boolean oddTime = true;//for debugging

void twoClapToggle() {
  boolean triggered = false;
  int firstWait = 150;
  int secondWait = 500;
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
    delay(firstWait);//NOT WORKING, TODO: FIX, ONLY WORKS THE FIRST TIME, THEN IT NEVER SENSES IF THE NORMAL VOLUME IS RANGE EVEN IF IT IS
    if (abs(normalVolume - getVolumeAnalog()) <= 2) {
      if (DEBUG) {
        Serial.println("The value of sound is within the range of normal.");
        Serial.println("Delaying for ");
        Serial.print(secondWait);
        Serial.println();
      }
      long currentTime = millis();
      while (millis() - currentTime <= secondWait && !triggered) {//TO fix, try setting the normal volume again afterwards, for some reason it goes up by 10 after the first time
        int secondVolume = getVolumeAnalog();
        if (normalVolume - secondVolume >= soundDifference) {
          if (DEBUG) {
            Serial.println("The second peak value of the sound is ");
            Serial.print(secondVolume);
            Serial.println();
          }
          reverseRelay();
          triggered = true;
          delay(triggerDelay);
          if(oddTime) {
            normalVolume += 10;//This is necessary when testing with a test LED or any change in energy, since when an LED is connected, the valeus change!
          }
          else {
            normalVolume -= 10;
          }
          oddTime = !oddTime;//for debugging
        }
      }
    }
      //delay(10000);//testing stuff
  //setNormalVolume();
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

