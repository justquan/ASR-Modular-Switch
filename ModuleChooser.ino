void sensorChooser() {
  //setModuleIndexFromBT();
  if (moduleIndex == 1) {
    lightSwitch();
  }

  else if (moduleIndex == 2) {
    infraredSwitch();
  }

  else if (moduleIndex == 3) {
    temperatureSwitch();
  }

  else if (moduleIndex == 4) {
    motionSwitch();
  }
  else if (moduleIndex == 5) {
    smokeSwitch();
  }
  else if (moduleIndex == 6) {
    soundSwitch();
  }

  //TODO: create proper system to determine what module is being used to use appropriate methods in code
}

//Reads input from bluetooth sensor and sets char moduleIndex to the received data
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
    Serial.println(lastFourData);//prints the last 4 chars of the command
    interpretCommand(lastFourData);
    command = "";//reset command to be an empty String
  }
}

//to interpret the 4 character String received as data via BT
void interpretCommand (String dataBT) {

  //sets trigger state to 1, which is ON
  if (dataBT.equals("GT01")) {
    triggerState = 1;
  }

  //sets trigger state to 0 , which is OFF
  else if (dataBT.equals("GT00")) {
    triggerState = 0;
  }

  //sets trigger state to 2, which is STROBE
  else if (dataBT.equals("GT00")) {
    triggerState = 2;
  }
  // if L is the first character of the command, set the moduleIndex to 0 so the microcontroller is using the light sensor, and parses rest of data for the lightInterpret to use.
  else if (dataBT.charAt(0) == 'L') {
    moduleIndex = 0;
    lightInterpret(dataBT.substring(1));
  }

  // if I is the first character of the command, set the moduleIndex to 1 so the microcontroller is using the infrared sensor, and parses rest of data for the infraredInterpret to use.
  else if (dataBT.charAt(0) == 'I') {
    moduleIndex = 1;
    infraredInterpret(dataBT.substring(1));
  }

  // if M is the first character of the command, set the moduleIndex to 2 so the microcontroller is using the motion sensor, and parses rest of data for the motionInterpreter to use.
  else if (dataBT.charAt(0) == 'M') {
    moduleIndex = 2;
    motionInterpret(dataBT.substring(1));
  }

  // if S is the first character of the command, set the moduleIndex to 3 so the microcontroller is using the smoke sensor, and parses rest of data for the smokeInterpreter to use.
  else if (dataBT.charAt(0) == 'S') {
    moduleIndex = 3;
    smokeInterpret(dataBT.substring(1));
  }

  // if V is the first character of the command, set the moduleIndex to 4 so the microcontroller is using the smoke sensor, and parses rest of data for the smokeInterpreter to use.
  else if (dataBT.charAt(0) == 'V') {
    moduleIndex = 4;
    volumeInterpret(dataBT.substring(1));
  }

  // if T is the first character of the command, set the moduleIndex to 5 so the microcontroller is using the temperature sensor, and parses rest of data for the temperatureInterpreter to use.
  else if (dataBT.charAt(0) == 'T') {
    moduleIndex = 5;
    temperatureInterpret(dataBT.substring(1));
  }
}

