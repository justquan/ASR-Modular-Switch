
void sensorChooser() {//when the switch is not in setup mode, this method is called consistently in the main loop
  switch (moduleIndex) {
    case -1://general
      generalTimerSwitch();//TODO: untested
      break;
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
}

//Reads input from bluetooth sensor and calls interpretCommand for the last four characters
void receiveData() {
  if (BT.available() > 0) {
    command = "";//see program BT_receiving_Commands
    //Serial.println(dataFromBT);/if you want to see ALL commands
  }
  while (BT.available() > 0) {//while there is a message
    command = BT.readString();//to return an int, just use read()
  }
  if (!command.equals("")) {//if command doesn't equal nothing
    String lastFourData = command.substring(command.length() - 4);//the last 4 letters of the command
    if (DEBUG) {
      Serial.print("Received 4 char command:");
      Serial.println(lastFourData);//prints the last 4 chars of the command
    }
    interpretCommand(lastFourData);
    command = "";//reset command to be an empty String
    btConnectionMade = true;
    statusLEDOff();
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
      if (!remainingString.equals("DCT")) {//aboslutely necessary, fixes annoying bug, so that when you press the disconnect button, it remembers the actual moduleIndex
        moduleIndex = -1;//so no sensor is chosen
      }
      generalInterpret(remainingString);
      break;
    // if L is the first character of the command, set the moduleIndex to 0 so the microcontroller is using the light sensor, and parses rest of data for the lightInterpret to use.
    case 'L':
      moduleIndex = 1;
      lightInterpret(remainingString);
      break;
    // if I is the first character of the command, set the moduleIndex to 1 so the microcontroller is using the infrared sensor, and parses rest of data for the infraredInterpret to use.
    case 'I':
      moduleIndex = 2;
      infraredInterpret(remainingString);
      break;
    // if M is the first character of the command, set the moduleIndex to 2 so the microcontroller is using the motion sensor, and parses rest of data for the motionInterpreter to use.
    case 'M':
      moduleIndex = 3;
      motionInterpret(remainingString);
      break;
    // if S is the first character of the command, set the moduleIndex to 3 so the microcontroller is using the smoke sensor, and parses rest of data for the smokeInterpreter to use.
    case 'S':
      moduleIndex = 4;
      smokeInterpret(remainingString);
      break;
    // if V is the first character of the command, set the moduleIndex to 4 so the microcontroller is using the smoke sensor, and parses rest of data for the smokeInterpreter to use.
    case 'V':
      moduleIndex = 5;
      volumeInterpret(remainingString);
      break;
    // if T is the first character of the command, set the moduleIndex to 5 so the microcontroller is using the temperature sensor, and parses rest of data for the temperatureInterpreter to use.
    case 'T':
      moduleIndex = 6;
      dhtInterpret(remainingString);//replaces temperature with dht here
      break;
    default:
      if (DEBUG) {
        Serial.print("No module matches 1st char. 1st char: ");
        Serial.println(firstChar);
      }
      break;
  }
}

//IMPORTANT: with all analog sensors, the data will be somewhat inaccurate with the HC-06 on, so maybe have to offset or take average or both (TODO)
void sendData() {
  if (timeElapsedSendBT >= sendDataIntervalMillis && btConnectionMade) {
    timeElapsedSendBT = 0;
    switch (moduleIndex) {
      case 1:
        btPrintLight();
//        printLightData();
        break;
      case 2:
        //relies on btprinting integrated into storeIRCode()
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
  }
}

//function run while in setup mode to run any necessary functions related to setting up usage with a sensor module
void runSetupProcesses() {
  checkSleep();
  switch (moduleIndex) {
    case 1:
      break;
    case 2:
      storeIRCode();
      break;
    case 3://motion
      break;
    case 4://smoke
      break;
    case 5://sound
      break;
    case 6://dht
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

