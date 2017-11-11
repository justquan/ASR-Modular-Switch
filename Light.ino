
//returns int data received by the light sensor and prints it
//setup:
//photoresistor end 1 to GND
//photoresistor end 2 to Analog pin 0
//10k resistor end 1 to Analog pin 0
//10k resistor end 2 to +5V

//a higher value for lightData means there is more light and less resistance
int getLightData() {
  int lightData = analogRead(lightPin);
  return lightData;
}

void printLightData() {
  Serial.println("lightData: ");//Don't condense print statements into one line or else error
  BT.println(getLightData());
  Serial.println (" ");
}
void lightSwitch() {
  delay(500);// for debugging to read data
  printLightData();//for debugging

  if (getLightData() < lightThresh && !relayState) {
    openRelay();
    BT.println("It's bright");//debugging

  }
  else if (getLightData() >= lightThresh && relayState) {
    closeRelay(); //open and closed must be closed might be reversed
    BT.println("It's dark");//debugging

  }
}

