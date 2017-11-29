void manualBTSensorChooser() {
  //setModuleIndexFromBT();

  if (moduleIndex == 1) {
    lightSwitch();
  }

  else if (moduleIndex == 2) {
    //motionSwitch();//don't use right now because if select 2, the delay for motionSwitch() makes it so you can't switch back
  }

  else if (moduleIndex == 3) {
    temperatureSwitch(70); // TODO: make it so that user inputs threshold temp
  }

  else if (moduleIndex == 4) {

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
  if (dataBT.equals("TOFF")) {
    triggerState = false;
  }

  if (dataBT.equals("T_ON")) {
    triggerState = true;
  }
}

