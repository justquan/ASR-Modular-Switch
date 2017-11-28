void manualBTSensorChooser() {
  setModuleIndexFromBT();

  if (moduleIndex == '5') {
    lightSwitch();
  }

  else if (moduleIndex == '2') {
    //motionSwitch();//don't use right now because if select 2, the delay for motionSwitch() makes it so you can't switch back
  }

  else if (moduleIndex == '3') {
    temperatureSwitch(70); // TODO: make it so that user inputs threshold temp
  }

  else if (moduleIndex == '4') {

  }

  else if (moduleIndex.equals("ON")) {//debug NEED TO CHANGE TO SOMETHING OTHER THAN MODULE INDEX BC IT IS A CHAR AND TO IS A STRING??
    Serial.println("ON");//debugging
    digitalWrite(LED_BUILTIN, HIGH);//debugging
    //TODO: use with ints maybe to make it more efficient and easier to work with. Also, store values into EEPROM and make a "key" for interpreting EEPROM values and at what spot
  }
  else if (moduleIndex.equals("OFF")) {//debug
    Serial.println("OFF");
    digitalWrite(LED_BUILTIN, LOW);
  }
  moduleIndex = "";//to reset moduleIndex value

  //TODO: create proper system to determine what module is being used to use appropriate methods in code
}

//Reads input from bluetooth sensor and sets char moduleIndex to the received data
void setModuleIndexFromBT() {
  if (BT.available() > 0) {
    command = "";//see program BT_receiving_Commands
  //Serial.println(dataFromBT);//debug to return ALL commands
  }
  while (BT.available() > 0) {
    command = BT.readString();//to return an int, just use read()
  }
  if (!command.equals("")) {//if command doesn't equal nothing 
    String lastThreeData = command.substring(command.length() - 3);//the last 3 letters of the command
    Serial.println(lastThreeData);//prints the last 3 letters of the command
    command = "";
  }
}

//to interpret the three character String received as data via BT
void interpretCommand (String dataBT){
  
}

