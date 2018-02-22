void sensorChooser() {//when the switch is not in setup mode, this method is called consistently in the main loop
  switch (moduleIndex) {
    case 1:
      lightSwitch();
      break;
    case 2:
      infraredSwitch();
      break;
    case 3:
      motionSwitch();
      break;
    case 4:
      smokeSwitch();
      break;
    case 5:
      soundSwitch();
      break;
    case 6:
      dhtSwitch();//not using temperatureSwitch
      break;
    default://if moduleIndex is not between 1 and 6 inclusive
      break;
  }


  //  if (moduleIndex == 1) {
  //    lightSwitch();
  //  }
  //
  //  else if (moduleIndex == 2) {
  //    infraredSwitch();
  //  }
  //
  //  else if (moduleIndex == 3) {
  //    motionSwitch();
  //  }
  //
  //  else if (moduleIndex == 4) {
  //    smokeSwitch();
  //  }
  //  else if (moduleIndex == 5) {
  //    soundSwitch();
  //  }
  //
  //  else if (moduleIndex == 6) {
  //    //    temperatureSwitch();
  //    dhtSwitch();//switched out temperature with dht
  //  }


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

//IMPORTANT: with all analog sensors, the data will be somewhat inaccurate with the HC-06 on, so maybe have to offset or take average or both (TODO)
void sendData() {

  if (timeElapsed > sendDataIntervalMillis) {
    timeElapsed = 0;
    switch (moduleIndex) {
      case 1:
        btPrintLight();
        break;
      case 2:
        btPrintIR();
        break;
      case 3:
        btPrintMotion();
        break;
      case 4:
        btPrintSmoke();
        break;
      case 5:
        btPrintSound();
        break;
      case 6:
        btPrintDHT();//not using temperatureSwitch
        break;
      default://if moduleIndex is not between 1 and 6 inclusive
        break;
    }
    //    if (moduleIndex == 1) {
    //      btPrintLight();
    //    }
    //
    //    else if (moduleIndex == 2) {
    //      btPrintIR();
    //    }
    //
    //    else if (moduleIndex == 3) {
    //      btPrintMotion();
    //    }
    //
    //    else if (moduleIndex == 4) {
    //      btPrintSmoke();
    //    }
    //    else if (moduleIndex == 5) {
    //      btPrintSound();
    //    }
    //    else if (moduleIndex == 6) {
    //      //    temperatureSwitch();
    //      btPrintDHT();//switched out temperature with dht
    //    }
  }
}

//Parameter: 4 character String filtered from receiveData()
//depending on the first letter of the command, sets the moduleIndex to the corresponding number
//takes the rest of data for the interpret function of the specified sensor to use
void interpretCommand (String dataBT) {
  char firstChar = dataBT.charAt(0);
  String remainingString = dataBT.substring(1);
  switch (firstChar) {
    case 'G':
      moduleIndex = -1;//so no sensor is chosen
      generalInterpret(remainingString);
      break;
    case 'L':
      moduleIndex = 1;
      lightInterpret(remainingString);
      break;
    case 'I':
      moduleIndex = 2;
      infraredInterpret(remainingString);
      break;
    case 'M':
      moduleIndex = 3;
      motionInterpret(remainingString);
      break;
    case 'S':
      moduleIndex = 4;
      smokeInterpret(remainingString);
      break;
    case 'V':
      moduleIndex = 5;
      volumeInterpret(remainingString);
      break;
    case 'T':
      moduleIndex = 6;
      dhtInterpret(remainingString);//replaces temperature with dht here
    default:
      break;
  }
  //  if (dataBT.equals("GOFF")) {
  //    //    digitalWrite(testLed, LOW);
  //    openRelay();
  //  }
  //
  //  //debugging with LED
  //  if (dataBT.equals("G_ON")) {
  //    closeRelay();
  //  }
  //  //sets triggerStateClosed to true, so when triggered, it closes the relay
  //  else if (dataBT.equals("GT01")) {
  //    triggerStateIsClose = true;
  //  }
  //
  //  //sets triggerStateClosed to false, so when triggered, it opens the relay
  //  else if (dataBT.equals("GT00")) {
  //    triggerStateIsClose = false;
  //  }
  //
  //  //if receives yes strobe command, then set strobeIfTriggered to true
  //  else if (dataBT.equals("GYSB")) {
  //    strobeIfTriggered = true;//TODO: Add strobing feature. Harder than I thought TODO: implement strobe
  //  }
  //
  //  //if receives no strobe command, then set strobeIfTriggered to false
  //  else if (dataBT.equals("GNSB")) {
  //    strobeIfTriggered = false;//
  //  }
  //
  //  else if (dataBT.equals("GDCT")) {//When disconnected
  //    btPowerOff();
  //    delay(1000);//gives time to turn off to prevent interference in analog signals
  //    setupSwitch = false;
  //  }
  //
  // if L is the first character of the command, set the moduleIndex to 0 so the microcontroller is using the light sensor, and parses rest of data for the lightInterpret to use.
  //  else if (dataBT.charAt(0) == 'L') {
  //    moduleIndex = 1;
  //    lightInterpret(dataBT.substring(1));
  //  }
  //
  //  // if I is the first character of the command, set the moduleIndex to 1 so the microcontroller is using the infrared sensor, and parses rest of data for the infraredInterpret to use.
  //  else if (dataBT.charAt(0) == 'I') {
  //    moduleIndex = 2;
  //    infraredInterpret(dataBT.substring(1));
  //  }
  //
  //  // if M is the first character of the command, set the moduleIndex to 2 so the microcontroller is using the motion sensor, and parses rest of data for the motionInterpreter to use.
  //  else if (dataBT.charAt(0) == 'M') {
  //    moduleIndex = 3;
  //    motionInterpret(dataBT.substring(1));
  //  }
  //
  //  // if S is the first character of the command, set the moduleIndex to 3 so the microcontroller is using the smoke sensor, and parses rest of data for the smokeInterpreter to use.
  //  else if (dataBT.charAt(0) == 'S') {
  //    moduleIndex = 4;
  //    smokeInterpret(dataBT.substring(1));
  //  }
  //
  //  // if V is the first character of the command, set the moduleIndex to 4 so the microcontroller is using the smoke sensor, and parses rest of data for the smokeInterpreter to use.
  //  else if (dataBT.charAt(0) == 'V') {
  //    moduleIndex = 5;
  //    volumeInterpret(dataBT.substring(1));
  //  }
  //
  //  // if T is the first character of the command, set the moduleIndex to 5 so the microcontroller is using the temperature sensor, and parses rest of data for the temperatureInterpreter to use.
  //  else if (dataBT.charAt(0) == 'T') {
  //    moduleIndex = 6;
  //    dhtInterpret(dataBT.substring(1));//replaces temperature with dht here
  //    //   temperatureInterpret(dataBT.substring(1));
  //  }
}

//function run while in setup mode to run any necessary functions related to setting up usage with a sensor module
void runSetupProcesses() {
  timeElapsed = 0;
  switch (moduleIndex) {
    case 1:
      btPrintLight();
      break;
    case 2:
      storeIRCode();//TODO Feb 21, implement
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    default://if moduleIndex is not between 1 and 6 inclusive
      break;
  }
}

long convertCommandToLong(String stringCommand) {
  int longCommand;
  while (stringCommand.indexOf("_") != -1) {
    stringCommand = stringCommand.substring(stringCommand.indexOf("_") + 1);//the stringCommand is equal to the string with the first "_" removed
  }
  longCommand = stringCommand.toInt();
  return longCommand;
}

