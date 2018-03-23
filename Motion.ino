//PIR Pinout:
//Lense facing away from you:
//Left Pin: 5V
//Middle Pin: Signal
//Right Pin: GND

//receives the three char command from the interpretCommand(), and makes sense of it
void motionInterpret(String filteredData) {//TODO: add ability to change motionInterval based on number from the android app
  //assuming that the received data is an integer in a string form
  motionInterval = convertCommandToLong(filteredData) * timerMillisPerUnit;//The last three numbers are multipied by timerMillisPerUnit
  if (DEBUG) {
    Serial.println("motionInterval = " + String(motionInterval));//String(motionInterval) to make the int a String for displaying
  }
}

boolean isMotion() {
  if (digitalRead(pirPin) == HIGH) {
    return true;
  }
  else {
    return false;
  }
}

//for debugging
void printMotionData() {
  if (isMotion()) {
    Serial.println("Motion!");
  }
  else {
    Serial.println("No motion");
  }
}

void motionSwitch() {
  if (timeElapsed2 >= motionSamplingInterval) {
    timeElapsed2 = 0;//reset
    if (DEBUG) {
      printMotionData();
    }
    if (timeElapsed >= motionInterval) {
      if (isMotion()) {
        trigger();
        timeElapsed = 0;
      }
      else {
        dormant();
      }
    }
  }
}

void btPrintMotion() {
  boolean motionStatus = isMotion();
  if (motionStatus != motionLastCheck) {//if status different from last time. default is motionLastCheck = false, which means no motion
    if (motionStatus) {
      BT.println(btSendBlock + "Motion");
    }
    else {
      BT.println(btSendBlock + "No Motion");
    }
  }
  motionLastCheck = motionStatus;
}

