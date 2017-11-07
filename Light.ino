
//returns int data received by the light sensor and prints it
//setup:
//photoresistor end 1 to 5v
//photoresistor end 2 to Analog pin 0
//photoresistor end 2 to 10k resistor
//10k resistor to ground
 
//a higher value for lightData means there is more light and less resistance
int getLightData(){
  int lightData = analogRead(lightPin);
  Serial.println("lightData: ");//Don't condense print statements into one line or else error
  Serial.print(lightData);
  Serial.println (" ");
  return lightData;
}

void lightSwitch(){
  delay(100);
 
  if (getLightData() < lightThresh && !relayState){
    openRelay();
  }
  else if(getLightData() >= lightThresh && relayState) {
    closeRelay(); //open and closed must be closed might be reversed
  }
}

