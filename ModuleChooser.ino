void manualBTSensorChooser() {
  setModuleIndexFromBT();

  if (moduleIndex == '1') {
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

  //TODO: create proper system to determine what module is being used to use appropriate methods in code
}

//Reads input from bluetooth sensor and sets char moduleIndex to the received data
void setModuleIndexFromBT() {
  if (BT.available()) {
    char dataFromBT = BT.read();
    moduleIndex = dataFromBT;
  }
}

