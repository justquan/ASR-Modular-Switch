
//ISSUE: still delays, false positives

//PIR Pinout:
//Lense facing away from you:
//Left Pin: 5V
//Middle Pin: Signal
//Right Pin: GND

//receives the three char command from the interpretCommand(), and makes sense of it
void motionInterpret(String filteredData) {//TODO: add ability to change motionInterval based on number from the android app
  //assuming that the received data is an integer in a string form
  motionInterval = convertCommandToLong(filteredData) * 1000;//The last three numbers are multipied by 1000 and set to the motionInterval, so if a value of 60 is received by example,
  if (DEBUG) {
    Serial.println("Motion Interval has been set to " + String(motionInterval));//String(motionInterval) to make the int a String for displaying
  }
  String stringMessage = "X" + String(motionInterval);
  BT.print(stringMessage);//for debugging / testing android handler, use print instead of write for full strings. ISSUE: for some reason, on app, only display a portion of the numbers sent. Maybe not fasat enough.

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
    Serial.println("Motion detected!");
  }
  else {
    Serial.println("No motion");
  }
}

void motionSwitch() {
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

void btPrintMotion() {
  if (isMotion()) {
    BT.println("Motion");
  }
  else {
    BT.println("No Motion");
  }
}

