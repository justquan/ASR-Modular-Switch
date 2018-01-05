//Note: When looking at analog sound values, a lower number means higher volume.

//receives the three char command from the interpretCommand(), and makes sense of it
void volumeInterpret(String filteredData) {//untested
  //assuming it is an integer 1-10
  setNormalVolume();
  soundDifference = (int)convertCommandToLong(filteredData);
  if(DEBUG) {
    Serial.println("Sound Difference value is set to ");
    Serial.print(soundDifference);
    Serial.println();
  }
}
//TODO: implement two clap feature
void soundSwitch() {
  int currentVolume = getVolumeAnalog();
  if (normalVolume - currentVolume >= soundDifference) { //If the currentvolume has a volume lower than the normal volume by aoundDifference units, it is triggered.
    if(DEBUG) {
      Serial.println("The initial value of the sound that just triggered the relay is ");
      Serial.print(currentVolume);
      Serial.println();
    }
    reverseRelay();//toggle relay
    delay(1000);//TODO: Use millis() timer instead of delay(), because delay() delays all other processes in addition to soundSwitch
    //delay is to avoid debouncing, the switch shouldn't listen to data right after it receives triggering data
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

