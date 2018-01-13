
//returns int data received by the light sensor and prints it

//setup:
//photoresistor end 1 to GND
//photoresistor end 2 to Analog pin 0
//10k resistor end 1 to Analog pin 0
//10k resistor end 2 to +5V

//receives the three char command from the interpretCommand(), and makes sense of it
//assumes that filteredData is a String representation of an integer
void lightInterpret(String filteredData) {
  lightThresh = filteredData.toInt();
}

//a higher value for lightData means there is more light and less resistance
int getLightData() {
  int lightData = analogRead(photoresistorPin);
  return lightData;
}

void printLightData() {
  Serial.println("lightData: ");//Don't condense print statements into one line or else error
  Serial.println(getLightData());
  Serial.println (" ");
}
void lightSwitch() {
  delay(500);// for debugging to read data TODO: see if this is needed
  printLightData();//for debugging
  int currentLightVal = getLightData();
  if (currentLightVal < lightThresh) {
    dormant();
    if (DEBUG) {
      Serial.println("It's bright! Light value: " + currentLightVal);
    }
  }
  else {// if currentLightVal >= lightThresh
    trigger();
    if (DEBUG) {
      Serial.println("It's dark! Light value: " + currentLightVal);
    }
  }
}

