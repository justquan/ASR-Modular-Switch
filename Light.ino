//all works 3/4
//returns int data received by the light sensor and prints it

//setup:
//photoresistor end 1 to GND
//photoresistor end 2 to Analog pin 0
//10k resistor end 1 to Analog pin 0
//10k resistor end 2 to +5V

//receives the three char command from the interpretCommand(), and makes sense of it
//assumes that filteredData is a String representation of an integer
void lightInterpret(String filteredData) {
  if (filteredData.substring(1).equals("TWO")) { //TWO command means Trigger When Over lightValue (when it's dark)
    triggerWhenBright = true;
  }
  else if (filteredData.substring(1).equals("TWU")) { //TWU command means Trigger When under lightValue (when it's bright)
    triggerWhenBright = false;
  }
  else {
    lightThresh = filteredData.toInt();
  }
}

//a higher value for lightData means there is more light and less resistance
int getLightDataRaw() {
  int lightData = analogRead(photoresistorPin);
  return lightData;
}

//after adding the offset if needed
int getLightData() {
  int alteredLightVal = getLightDataRaw();
  if (relayClosed) {
    return alteredLightVal + relayAnalogValOffsetLight;
  }
  else {
    return alteredLightVal;
  }
}

//for debugging
void printLightData() {
  Serial.print("lightData: " + String(getLightData()));
//  Serial.println(getLightData());
}

void lightSwitch() {
  if (timeElapsed >= lightSamplingInterval) {
    timeElapsed = 0;
    int currentLightVal = getLightData();
    if (currentLightVal > lightThresh) {// if currentLightVal > lightThresh / it is dark
      if (DEBUG) {
        Serial.println("Bright! Light val:" + currentLightVal);
      }
      trigger();
    }
    else {//low value means bright
      dormant();
      if (DEBUG) {
        Serial.println("Dark! light val:" + currentLightVal);
      }
    }
  }
}

//
//void lightSwitch() {
//  if (timeElapsed >= lightSamplingInterval) {
//    timeElapsed = 0;
//    int currentLightVal = getLightData();
//    if (currentLightVal < lightThresh) {//low value means bright
//      if (DEBUG) {
//        Serial.println("Bright! Light val:" + currentLightVal);
//      }
//      if (triggerWhenBright) {//if set to trigger when bright
//        trigger();
//      }
//      else {//if set to trigger when dark
//        dormant();
//      }
//    }
//    else {// if currentLightVal > lightThresh / it is dark
//      if (DEBUG) {
//        Serial.println("Dark! light val:" + currentLightVal);
//      }
//      if (!triggerWhenBright) {//if set to trigger when dark
//        trigger();
//      }
//      else {//if set to trigger when bright
//        dormant();
//      }
//    }
//  }
//}

void btPrintLight() {
//  int times = 100;//takes average of 100 times
//  int total;
//  for (int i = 0; i < times; i++) {
//    total += getLightData();
//  }
//  total /= times;
  String msg = btSendBlock;
  msg.concat(getLightData());
  if(DEBUG) {
    Serial.print("BT Sending:");
    Serial.println(msg);
  }
  BT.println(msg);
}

