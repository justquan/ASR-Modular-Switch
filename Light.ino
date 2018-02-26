
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
    triggerWhenOverLightValue = true;
  }
  else if (filteredData.substring(1).equals("TWU")) { //TWU command means Trigger When under lightValue (when it's bright)
    triggerWhenOverLightValue = false;
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
  Serial.print("lightData: ");
  Serial.println(getLightData());
}

void lightSwitch() {
  int currentLightVal = getLightData();

  if (triggerWhenOverLightValue) {//when it is dark
    if (currentLightVal > lightThresh) {
      trigger();
      if (DEBUG) {
        Serial.println("It's dark! Light value: " + currentLightVal);
      }

    }
    else {// if currentLightVal < lightThresh
      dormant();
      if (DEBUG) {
        Serial.println("It's bright! Light value: " + currentLightVal);
      }
    }
  }

  else {// trigger when under light value (when it is bright
    if (currentLightVal < lightThresh) {
      trigger();
      if (DEBUG) {
        Serial.println("It's bright! Light value: " + currentLightVal);
      }

    }
    else {// if currentLightVal > lightThresh
      dormant();
      if (DEBUG) {
        Serial.println("It's dark! Light value: " + currentLightVal);
      }
    }
  }
}

void btPrintLight() {
  String msg = "X" + getLightData();
  BT.println(msg);
}

