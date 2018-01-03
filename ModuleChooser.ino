void sensorChooser() {//when the switch is not in setup mode, this method is called consistently in the main loop
  //setModuleIndexFromBT();
  if (moduleIndex == 1) {
    lightSwitch();
  }

  else if (moduleIndex == 2) {
    infraredSwitch();
  }

  else if (moduleIndex == 3) {
    motionSwitch();
  }

  else if (moduleIndex == 4) {
    smokeSwitch();
  }
  else if (moduleIndex == 5) {
    soundSwitch();
  }

  else if (moduleIndex == 6) {
    temperatureSwitch();
  }

  //TODO: create proper system to determine what module is being used to use appropriate methods in code
}

//Reads input from bluetooth sensor and calls interpretCommand for the last four characters
void receiveData() {
  if (BT.available() > 0) {
    command = "";//see program BT_receiving_Commands
    //Serial.println(dataFromBT);//debug to return ALL commands
  }
  while (BT.available() > 0) {
    command = BT.readString();//to return an int, just use read()
  }
  if (!command.equals("")) {//if command doesn't equal nothing
    String lastFourData = command.substring(command.length() - 4);//the last 4 letters of the command
    if (DEBUG) {
      Serial.println(lastFourData);//prints the last 4 chars of the command
    }
    interpretCommand(lastFourData);
    command = "";//reset command to be an empty String
  }
}

//to interpret the 4 character String received as data via BT
void interpretCommand (String dataBT) {

  //debugging with LED
  if (dataBT.equals("_OFF")) {
    digitalWrite(testLed, LOW);
  }

  //debugging with LED
  if (dataBT.equals("__ON")) {
    digitalWrite(testLed, HIGH);
    BT.write("AON~");//for debugging / displaying data to android attempt
    BT.print("AON~");//debug
    Serial.println("android should say AON~");//debug, 12/29 added ~ to mark the end of the message
  }
  //sets trigger state to 1, which is ON
  else if (dataBT.equals("GT01")) {
    triggerState = 1;
  }

  //sets trigger state to 0 , which is OFF
  else if (dataBT.equals("GT00")) {
    triggerState = 0;
  }

  //sets trigger state to 2, which is STROBE
  else if (dataBT.equals("GT02")) {
    triggerState = 2;
  }

  else if (dataBT.equals("GDCT")) {//When disconnected
    setupSwitch = false;
  }

  // if L is the first character of the command, set the moduleIndex to 0 so the microcontroller is using the light sensor, and parses rest of data for the lightInterpret to use.
  else if (dataBT.charAt(0) == 'L') {
    moduleIndex = 1;
    lightInterpret(dataBT.substring(1));
  }

  // if I is the first character of the command, set the moduleIndex to 1 so the microcontroller is using the infrared sensor, and parses rest of data for the infraredInterpret to use.
  else if (dataBT.charAt(0) == 'I') {
    moduleIndex = 2;
    infraredInterpret(dataBT.substring(1));
  }

  // if M is the first character of the command, set the moduleIndex to 2 so the microcontroller is using the motion sensor, and parses rest of data for the motionInterpreter to use.
  else if (dataBT.charAt(0) == 'M') {
    moduleIndex = 3;
    motionInterpret(dataBT.substring(1));
  }

  // if S is the first character of the command, set the moduleIndex to 3 so the microcontroller is using the smoke sensor, and parses rest of data for the smokeInterpreter to use.
  else if (dataBT.charAt(0) == 'S') {
    moduleIndex = 4;
    smokeInterpret(dataBT.substring(1));
  }

  // if V is the first character of the command, set the moduleIndex to 4 so the microcontroller is using the smoke sensor, and parses rest of data for the smokeInterpreter to use.
  else if (dataBT.charAt(0) == 'V') {
    moduleIndex = 5;
    volumeInterpret(dataBT.substring(1));
  }

  // if T is the first character of the command, set the moduleIndex to 5 so the microcontroller is using the temperature sensor, and parses rest of data for the temperatureInterpreter to use.
  else if (dataBT.charAt(0) == 'T') {
    moduleIndex = 6;
    temperatureInterpret(dataBT.substring(1));
  }
}

