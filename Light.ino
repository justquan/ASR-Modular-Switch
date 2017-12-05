
//returns int data received by the light sensor and prints it
//setup:
//photoresistor end 1 to GND
//photoresistor end 2 to Analog pin 0
//10k resistor end 1 to Analog pin 0
//10k resistor end 2 to +5V

//receives the three char command from the interpretCommand(), and makes sense of it
void lightInterpret(String filteredData) {

}

//a higher value for lightData means there is more light and less resistance
int getLightData() {
  int lightData = analogRead(lightPin);
  return lightData;
}

void printLightData() {
  Serial.println("lightData: ");//Don't condense print statements into one line or else error
  Serial.println(getLightData());
  Serial.println (" ");
}
void lightSwitch() {
  delay(500);// for debugging to read data
  printLightData();//for debugging

  if (getLightData() < lightThresh && !relayState) {
    openRelay();
    Serial.println("It's bright");//debugging

  }
  else if (getLightData() >= lightThresh && relayState) {
    closeRelay(); //open and closed must be closed might be reversed
    Serial.println("It's dark");//debugging

  }
}

